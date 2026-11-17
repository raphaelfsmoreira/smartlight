#pragma once
#include <Arduino.h>
#include "lampada.h"
#include "encoder_rotativo.h"
#include "botao.h"

enum class PedidoUsuario{
  Nenhum,
  Pareamento
};

class ControleFisico {
  public:
    ControleFisico(Lampada& lampada);

    void iniciar();

    PedidoUsuario ouvir();
  
  private:
    Lampada& _lampada;
    EncoderRotativo _encoder;
    Botao _botao;
};