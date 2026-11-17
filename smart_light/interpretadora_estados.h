#pragma once
#include <Arduino.h>
#include "lampada.h"


class InterpretadoraDeEstado {
  public:
    InterpretadoraDeEstado(Lampada& lampada);

    // Aplica um JSON de estado desejado. Retorna false se o JSON for inválido.
    bool receberEstado(const String& json);

    // Devolve o estado atual da lâmpada em JSON
    String descreverEstado() const;

  private:
    Lampada& _lampada;
};