#include "encoder_rotativo.h"

EncoderRotativo::EncoderRotativo(uint8_t pinoClk, uint8_t pinoDt)
  : _pinoClk(pinoClk),
    _pinoDt(pinoDt),
    _ultimoClk(HIGH) {}

void EncoderRotativo::iniciar() {
  pinMode(_pinoClk, INPUT_PULLUP);
  pinMode(_pinoDt, INPUT_PULLUP);
  _ultimoClk = digitalRead(_pinoClk);
  Serial.println("EncoderRotativo inicializado");
}

int EncoderRotativo::lerGiro() {
  int clkAtual = digitalRead(_pinoClk);

  // CLK não mudou: não girou
  if (clkAtual == _ultimoClk) {
    return 0;
  }

  _ultimoClk = clkAtual;

  // Conta somente a borda de descida
  if (clkAtual != LOW) {
    return 0;
  }

  // O DT diz o sentido
  return (digitalRead(_pinoDt) == HIGH) ? 1 : -1;
}