#pragma once

#include <Arduino.h>
#include "../lampada/interpretadora_estados.h"
#include "../rede/conexao_wifi.h"


/*
  COMANDOS PARA SEREM INSERIDOS NO TERMINAL PARA DEBUGGING.
  
  Comandos suportados:

  - Escrever estado da lâmpada. 
  Digitar em formato json, do tipo:
  {
    "ligado":true, 
    "brilho":<INT>,
    "rgb":[<0-255>, <0-255>, <0-255]
  }

  - Inserir novas credenciais de WiFi
  (lembrando que o ESP32 só suporta conexões 2.4GHz)

  COMANDO:
  wifi <ssid_da_rede>;<senha>

  - Descrever estado da lâmpada e conexão wifi.
  COMANDO:
  status

  - Reiniciar a placa via software.
  COMANDO:
  reiniciar

  - Apaga as credenciais de wifi
  COMANDO:
  esquecer

*/

class ConsoleSerial {
  public:
    ConsoleSerial(InterpretadoraDeEstado& interpretadora, 
                  ConexaoWifi& conexaoWifi);

    void ouvir();

  private:
    InterpretadoraDeEstado& _interpretadora;
    ConexaoWifi& _conexaoWifi;

    String _buffer;
    static constexpr unsigned int TAMANHO_MAXIMO_LINHA = 200;

    void executar(const String& linha);
    void comandoWifi(const String& argumentos);
    void comandoStatus();
    void comandoEsquecer();
};