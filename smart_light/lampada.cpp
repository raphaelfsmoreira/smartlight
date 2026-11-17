#include "lampada.h"
#include "config.h"

Lampada::Lampada()
  : _led(Config::Pinos::DIN_ANEL, Config::AnelRGB::QUANTIDADE_LEDS),
    _ligada(false),
    _brilho(Config::Lampada::BRILHO_INICIAL),
    _red(Config::Lampada::COR_INICIAL_R),
    _green(Config::Lampada::COR_INICIAL_G),
    _blue(Config::Lampada::COR_INICIAL_B)
    {}

// Método de inicialização. Ativa o led com as configs iniciais.
// Retorna ao setup false se a inicialização do PWM falhar.
bool Lampada::iniciar(){
  if (!_led.iniciar()){
    return false;
  }

  atualizarLed();
  return true;
}

// ---------------------------------------------------------
// ALTERNA ESTADOS: LIGA E DESLIGA
// ---------------------------------------------------------

void Lampada::ligar() {
  _ligada = true;
  atualizarLed();
}

void Lampada::desligar() {
  _ligada = false;
  atualizarLed();
}

// Para o caso do acionamento do botão do encoder
// O encoder não sabe se a lâmpada está ligada ou ligada
void Lampada::alternar(){
  _ligada = !_ligada;
  atualizarLed();
}

void Lampada::aoMudarEstado(std::function<void()> ouvinte){
  _ouvinte = ouvinte;
}

// ----------------------------------------------------------
// CONTROLE DE BRILHO
// ----------------------------------------------------------

void Lampada::definirBrilho(int valor){
  _brilho = constrain(valor, 
                      Config::Lampada::BRILHO_MINIMO, 
                      Config::Lampada::BRILHO_MAXIMO);

  atualizarLed();
}

void Lampada::alterarBrilho(int incremento){
  definirBrilho(_brilho + incremento);
}

//--------------------------------------------------------
// CONTROLE DE COR
//--------------------------------------------------------

void Lampada::definirCor(uint8_t red, uint8_t green, uint8_t blue){
  _red = red;
  _green = green;
  _blue = blue;

  atualizarLed();
}

//---------------------------------------------------------
// GETTERS
//---------------------------------------------------------

// O const na assinatura do método significa const correctness
// const no fim do método: promessa de que ele não altera o objeto.
// O compilador fiscaliza. Isso permite chamar esses métodos em
// objetos recebidos como const Lampada& (sem equivalente em Java).

bool Lampada::estaLigada() const {
  return _ligada;
}

uint8_t Lampada::getBrilho() const {
  return _brilho; // %
}

uint8_t Lampada::getRed() const {
  return _red;
}

uint8_t Lampada::getGreen() const {
  return _green;
}

uint8_t Lampada::getBlue() const {
  return _blue;
}

// Método que chama o driver do ledRgb.
void Lampada::atualizarLed() {

  if (!_ligada) {
    _led.apagar();
  } else {

    // Lembrando que a unidade de brilho, na interface, está em escala de porcentagem
    _led.escrever(_red   *  _brilho / 100,
                  _green *  _brilho / 100,
                  _blue  *  _brilho / 100);

  }

  // Toda mudança no led passa por esse método. Então quando essa função for chamada,
  // o ouvinte desse mudança (mqtt) irá executar.
  if(_ouvinte){
    _ouvinte();
  }

}