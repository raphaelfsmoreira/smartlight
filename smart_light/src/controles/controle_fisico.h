#pragma once
#include <Arduino.h>
#include "../lampada/lampada.h"
#include "../hardware/encoder_rotativo.h"
#include "../hardware/botao.h"

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