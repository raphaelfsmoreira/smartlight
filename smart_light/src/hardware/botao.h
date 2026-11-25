#pragma once
#include <Arduino.h>

enum class EventoBotao {
  Nenhum,
  CliqueSimples,
  CliqueLongo
};

class Botao {
  public:
    Botao(uint8_t pino);

    void iniciar();

    EventoBotao lerEvento();
  
  private:
    uint8_t _pino;

    // Atributos de debounce
    bool _ultimaLeitura;
    bool _estadoEstavel;
    unsigned long _ultimaMudanca;

    // Clique longo
    unsigned long _inicioPressionado;
    bool _longoDisparado;
};