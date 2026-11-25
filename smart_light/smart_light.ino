#include "src/config/config.h"
#include "src/lampada/lampada.h"
#include "src/controles/controle_fisico.h"
#include "src/lampada/interpretadora_estados.h"
#include "src/rede/conexao_wifi.h"
#include "src/pareamento/servidor_ble.h"
#include "src/rede/servico_mqtt.h"
#include "src/controles/console_serial.h"

Lampada lampada;
ControleFisico controles(lampada);
InterpretadoraDeEstado interpretadora(lampada);

ConexaoWifi conexaoWifi;
ServidorBle servidorBle;

ServicoMqtt servicoMqtt(interpretadora);
ConsoleSerial console(interpretadora, conexaoWifi);



void setup() {

  Serial.begin(115200);
  Serial.println("Inicializando SmartLight...");

  if(!lampada.iniciar()){
    Serial.println("Erro ao inicializar lâmpada");
    while(true) delay(1000);
  }

  Serial.println("Lâmpada inicializada com sucesso");

  controles.iniciar();

  // Callback que implementa o Throttle de publicação.
  lampada.aoMudarEstado([]() {
    servicoMqtt.marcarEstadoAlterado();
  });

  conexaoWifi.iniciar();

  Serial.println("Controles inicializados com sucesso");

  // Configura o servidor e o certificado.
  servicoMqtt.iniciar();

  Serial.println("Módulo Broker inicializado com sucesso");

  Serial.println("Setup concluído");
}

void loop() {

  conexaoWifi.manter();

  servicoMqtt.manter();

  if (controles.ouvir() == PedidoUsuario::Pareamento) {
    servidorBle.iniciar();
  }

  console.ouvir();
}