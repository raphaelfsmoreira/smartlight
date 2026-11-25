#pragma once
#include <Arduino.h>
#include "secrets.h"


// O config.h reúne a definição de todas as variáveis
// Os magic numbers do código são reunidos aqui.

namespace Config {

  // ---------------------------------------------------------
  // PINOS
  // ---------------------------------------------------------

  namespace Pinos {

    // Pinos do Rotary Encoder
    constexpr uint8_t CLK       = 21;  // Clock (rotação)
    constexpr uint8_t DT        = 22;  // Data (sentido)
    constexpr uint8_t SW        = 23;  // Switch (botão)

    constexpr uint8_t DIN_ANEL  = 25;


    /* Pinos do Led RGB. Deprecated.
    constexpr uint8_t RED   = 25;
    constexpr uint8_t GREEN = 26;
    constexpr uint8_t BLUE  = 27;
    */
    
  }

  // ----------------------------------------------------------
  // PWM (LEDC)
  // ----------------------------------------------------------

  namespace Pwm {
    constexpr uint32_t FREQUENCIA = 5000;  // Hz
    constexpr uint8_t  RESOLUCAO  = 8;     // bits -> valores de 0 a 255

    constexpr uint32_t VALOR_MAXIMO = (1u << RESOLUCAO) - 1;
  }


  namespace AnelRGB {
    constexpr uint16_t QUANTIDADE_LEDS = 12;
  }

  namespace Lampada {

    // O brilho é dado em escala de porcentagem
    constexpr uint8_t BRILHO_INICIAL  = 75; // %
    constexpr uint8_t BRILHO_MINIMO   = 10; // %
    constexpr uint8_t BRILHO_MAXIMO   = 100; // %

    constexpr uint8_t COR_INICIAL_R   = 255;
    constexpr uint8_t COR_INICIAL_G   = 255;
    constexpr uint8_t COR_INICIAL_B   = 255;
  }

  namespace Controles {
    constexpr int           PASSO_BRILHO  = 5; /// %
    constexpr unsigned long DEBOUNCE_MS   = 30;

    // Tempo de pressionamento longo para modo pareamento ser ativado
    constexpr unsigned long LONG_PRESS_MS = 4000;
  }

  namespace Wifi {
    constexpr unsigned long INTERVALO_RECONEXAO_MS = 10000;
  }

  namespace Ble {

    
  }

  namespace BrokerMQTT {

    constexpr const char*     BROKER                  = "t916f211.ala.us-east-1.emqxsl.com";
    constexpr uint16_t        PORTA                   = 8883;
    constexpr const char*     USERNAME                = Secrets::MQTT_USERNAME;
    constexpr const char*     PASSWORD                = Secrets::MQTT_PASSWORD;
    constexpr const char*     ID_CLIENTE              = "smartlight-raphael-01";
    constexpr const char*     TOPICO_CMD              = "smartlight/raphael-01/CMD";
    constexpr const char*     TOPICO_STATUS           = "smartlight/raphael-01/STATUS";
    constexpr const char*     TOPICO_DISPONIBILIDADE  = "smartlight/raphael-01/AVAILABILITY"; 
  }

}