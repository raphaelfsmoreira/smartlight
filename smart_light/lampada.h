#pragma once
#include <Arduino.h>
#include "led_rgb.h"

class Lampada {
  public:
    // Construtor
    Lampada();

    // Retorna false se houve falha na inicialização
    bool iniciar();

    void ligar();
    void desligar();
    void alternar();

    // Função de callback. Toda mudança na lâmpada irá executar a função de ouvinte.
    void aoMudarEstado(std::function<void()> ouvinte);

    void definirBrilho(int valor);
    void alterarBrilho(int incremento);

    void definirCor(uint8_t red, uint8_t green, uint8_t blue);

    // Getters
    bool estaLigada() const;
    uint8_t getBrilho() const;

    uint8_t getRed() const;
    uint8_t getGreen() const;
    uint8_t getBlue() const;

  private:
    // Agora o objeto led mora dentro da classe lampada (nao mais é acessado por
    // referência.
    LedRGB _led;

    bool _ligada;
    uint8_t _brilho;
    uint8_t _red, _green, _blue;
    std::function<void()> _ouvinte;

    void atualizarLed();
};