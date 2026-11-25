#pragma once
#include <Arduino.h>

class EncoderRotativo {
  public:
    EncoderRotativo(uint8_t pinoClk, uint8_t pinoDt);

    void iniciar();

    // Retorna +1 para giro horário, -1 para antihorário e 0 quando não há giro
    int lerGiro();

  private:
    uint8_t _pinoClk;
    uint8_t _pinoDt;
    int _ultimoClk;
};
