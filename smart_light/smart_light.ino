#include "config.h"
#include "lampada.h"
#include "controle_fisico.h"
#include "interpretadora_estados.h"
#include "conexao_wifi.h"
#include "servico_mqtt.h"
#include "console_serial.h"

Lampada lampada;
ControleFisico controles(lampada);
InterpretadoraDeEstado interpretadora(lampada);

ConexaoWifi conexaoWifi;

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
    Serial.println("Modo pareamento (em breve)");
  }

  console.ouvir();
}