#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedRGB{
  public:
      // Construtor da classe
      LedRGB(uint8_t pino_din, uint16_t quantidadeLeds);

      // Métodos só declarados. São indices da classe (nomeação)
      bool iniciar();
      void escrever(uint8_t red, uint8_t green, uint8_t blue);
      void apagar();

  // O _ em C++ funciona para diferenciar parâmetro de atributo...
  // Seria equivalente, em Java, a this.pinoVermelho = pinoVermelho.
  private:
    Adafruit_NeoPixel _anel;
};