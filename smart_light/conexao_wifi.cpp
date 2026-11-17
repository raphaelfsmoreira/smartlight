#include "conexao_wifi.h"
#include "config.h"
#include <WiFi.h>

ConexaoWifi::ConexaoWifi()
  : _estavaConectada(false),
    _ultimaTentativa(0),
    _motivoDesconexao(0) {}

void ConexaoWifi::iniciar() {

  // Callback: a biblioteca chama esta lambda sempre que o Wi-Fi desconecta
  WiFi.onEvent(
    [this](WiFiEvent_t evento, WiFiEventInfo_t info) {
      uint8_t motivo = info.wifi_sta_disconnected.reason;

      // ASSOC_LEAVE = fomos nós que chamamos WiFi.disconnect(). Não é erro.
      if (motivo != WIFI_REASON_ASSOC_LEAVE) {
        _motivoDesconexao = motivo;
      }
    },
    ARDUINO_EVENT_WIFI_STA_DISCONNECTED
  );

  // Conecta na rede salva, se houver
  conectar();

}

void ConexaoWifi::conectar() {
  // carregar() escreve direto em _ssid e _senha. Sem rede salva, ficam vazios.
  if (!_repositorio.carregar(_ssid, _senha)) {
    Serial.println("Nenhuma rede salva: Wi-Fi em modo offline");
    return;
  }

  WiFi.mode(WIFI_STA);
  Serial.printf("Conectando ao WiFi %s...\n", _ssid.c_str());
  tentarConectar();
}

void ConexaoWifi::trocarRede(const String& ssid, const String& senha) {
  _repositorio.salvar(ssid, senha);
  conectar();
}

void ConexaoWifi::tentarConectar() {
  _motivoDesconexao = 0;
  WiFi.disconnect();
  WiFi.begin(_ssid.c_str(), _senha.c_str());
  _ultimaTentativa = millis();
}

void ConexaoWifi::esquecer() {
  _repositorio.apagar();   // apaga a rede da flash

  _ssid = "";
  _senha = "";
  _estavaConectada = false;

  WiFi.disconnect(true);   // true = também desliga o rádio
  WiFi.mode(WIFI_OFF);

  Serial.println("Wi-Fi em modo offline.");
}

void ConexaoWifi::manter() {
  // Modo offline: nenhuma rede cadastrada, nada a manter.
  if (_ssid.isEmpty()) {
    return;
  }

  bool conectada = estaConectada();

  if (conectada && !_estavaConectada) {
    Serial.print("WiFi conectado com sucesso. IP: ");
    Serial.println(WiFi.localIP());

  } else if (!conectada && _estavaConectada) {
    Serial.printf("WiFi foi desconectado. %s\n", descreverMotivo(_motivoDesconexao));
  }
  _estavaConectada = conectada;

  // Sem rede: avisa o motivo e tenta de novo a cada INTERVALO_RECONEXAO_MS, sem travar o loop
  if (!conectada && millis() - _ultimaTentativa >= Config::Wifi::INTERVALO_RECONEXAO_MS) {
    Serial.printf("Falha no WiFi: %s (código %d). Tentando de novo...\n",
                  descreverMotivo(_motivoDesconexao), _motivoDesconexao);
    tentarConectar();
  }
}


const char* ConexaoWifi::descreverMotivo(uint8_t motivo) {
  switch (motivo) {
    case WIFI_REASON_NO_AP_FOUND:
      return "Rede não encontrada";

    case WIFI_REASON_AUTH_FAIL:
    case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
    case WIFI_REASON_HANDSHAKE_TIMEOUT:
      return "Senha incorreta";

    case WIFI_REASON_BEACON_TIMEOUT:
      return "Sinal WiFi perdido";

    case 0:
      return "Sem resposta da rede";

    default:
      return "Erro desconhecido";
  }
}

bool ConexaoWifi::estaConectada() const {
  return WiFi.status() == WL_CONNECTED;
}