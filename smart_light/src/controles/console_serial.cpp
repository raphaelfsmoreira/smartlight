#include "console_serial.h"

ConsoleSerial::ConsoleSerial(InterpretadoraDeEstado& interpretadora,
                             ConexaoWifi& conexaoWifi)
  : _interpretadora(interpretadora),
    _conexaoWifi(conexaoWifi) {}

void ConsoleSerial::ouvir() {

  // Lê só o que já chegou e volta pro loop
  while (Serial.available()) {
    
    char c = Serial.read();

    // Se houver quebra de linha, entende-se que o comando deve ser executado.
    if (c == '\n') {
      _buffer.trim();   // tira espaços e o '\r' que alguns terminais mandam

      if (_buffer.length() > 0) {
        executar(_buffer);
      }

      // Reseta o buffer como string vazia.
      _buffer = "";

    // Se não foi capturado uma quebra de linha (Enter) continua montando a string.
    } else if (_buffer.length() < TAMANHO_MAXIMO_LINHA) {
      // Vai montando a string de comando a cada linha digiitada até o máximo 
      // de tamanho da linha.
      _buffer += c;
    }
  }
}


void ConsoleSerial::executar(const String& linha) {
  if (linha.startsWith("{")) {

    // Lembrando que receberEstado retorna false se o formato do Json for inválido.
    if (!_interpretadora.receberEstado(linha)) {
      Serial.println("JSON de estado inválido");
      return;
    }
    Serial.println(_interpretadora.descreverEstado());

  } else if (linha.startsWith("wifi ")) {
    comandoWifi(linha.substring(5));   // pula os 5 caracteres de "wifi "

  } else if (linha == "status") {
    comandoStatus();

  } else if (linha == "reiniciar") {
    Serial.println("Reiniciando...");
    delay(300);
    ESP.restart();     

  } else if (linha == "esquecer") {
    comandoEsquecer();

  } else {
    Serial.println("Comando desconhecido. Use: {json} | wifi rede;senha | esquecer | status | reiniciar");
  }
}

void ConsoleSerial::comandoWifi(const String& argumentos) {
  int separador = argumentos.indexOf(';');
  if (separador < 0) {
    Serial.println("Formato: wifi rede;senha");
    return;
  }

  String ssid  = argumentos.substring(0, separador);
  String senha = argumentos.substring(separador + 1);

  _conexaoWifi.trocarRede(ssid, senha);

}

void ConsoleSerial::comandoStatus() {
  Serial.print("Lâmpada: ");
  Serial.println(_interpretadora.descreverEstado());

  Serial.print("WiFi: ");
  Serial.println(_conexaoWifi.estaConectada() ? "conectado" : "desconectado");
}

void ConsoleSerial::comandoEsquecer() {
  _conexaoWifi.esquecer();
  Serial.println("Rede apagada");
}