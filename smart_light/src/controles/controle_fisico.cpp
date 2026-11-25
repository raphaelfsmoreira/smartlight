#include "controle_fisico.h"
#include "../config/config.h"

ControleFisico::ControleFisico(Lampada& lampada)
  : _lampada(lampada),
    _encoder(Config::Pinos::CLK, Config::Pinos::DT),
    _botao(Config::Pinos::SW) {}

void ControleFisico::iniciar() {
  _encoder.iniciar();
  _botao.iniciar();
  Serial.println("Controles físicos inicializados");
}

PedidoUsuario ControleFisico::ouvir() {
  // ---------- Encoder: brilho ----------
  int giro = _encoder.lerGiro();

   // Com a lâmpada desligada o giro é ignorado
  if (giro != 0 && _lampada.estaLigada()) {
    _lampada.alterarBrilho(giro * Config::Controles::PASSO_BRILHO);
    Serial.printf("Brilho: %d%%\n", _lampada.getBrilho());
  }


  // ---------- Botão: liga/desliga e pareamento ----------
  EventoBotao evento = _botao.lerEvento();

  if (evento == EventoBotao::CliqueSimples) {
    _lampada.alternar();
    Serial.println(_lampada.estaLigada() ? "Estado: Ligado": "Estado: Desligado");
  }

  if (evento == EventoBotao::CliqueLongo) {
    return PedidoUsuario::Pareamento;
  }

  return PedidoUsuario::Nenhum;
}