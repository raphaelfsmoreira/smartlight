#include "botao.h"
#include "../config/config.h"

Botao::Botao(uint8_t pino)
  : _pino(pino),
    _ultimaLeitura(HIGH),
    _estadoEstavel(HIGH),
    _ultimaMudanca(0),
    _inicioPressionado(0),
    _longoDisparado(false) {}

void Botao::iniciar() {
  pinMode(_pino, INPUT_PULLUP);
  Serial.println("Botão inicializado");
}

EventoBotao Botao::lerEvento() {
  bool leitura = digitalRead(_pino);
  unsigned long agora = millis();

  // ---------- 1. Debounce ----------
  if (leitura != _ultimaLeitura) {
    _ultimaMudanca = agora;
    _ultimaLeitura = leitura;
  }

  bool estabilizou = (agora - _ultimaMudanca >= Config::Controles::DEBOUNCE_MS);

  if (estabilizou && leitura != _estadoEstavel) {
    _estadoEstavel = leitura;

    if (_estadoEstavel == LOW) {
      // Apertou: começa a contar o tempo
      _inicioPressionado = agora;
      _longoDisparado = false;
    } else if (!_longoDisparado) {
      // Soltou antes do tempo do clique longo
      return EventoBotao::CliqueSimples;
    }
  }

  // ---------- 2. Clique longo ----------
  bool segurando = (_estadoEstavel == LOW);
  bool passouDoTempo = (agora - _inicioPressionado >= Config::Controles::LONG_PRESS_MS);

  if (segurando && !_longoDisparado && passouDoTempo) {
    _longoDisparado = true;
    return EventoBotao::CliqueLongo;
  }

  return EventoBotao::Nenhum;
}