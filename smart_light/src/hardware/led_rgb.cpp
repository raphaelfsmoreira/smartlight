#include "led_rgb.h"

/*
===============================================================================
Detalhes do Driver do Anel WS2812:
===============================================================================

- TERCEIRO PARÂMETRO DE INSTANCIAÇÃO DO ANEL (PONTO DE ATENÇÃO NA CONSTRUÇÃO DO PROTÓTIPO)

NEO_GRB: ordem das cores. Cada LED recebe 3 bytes em sequência, um por cor. O WS2812B 
espera primeiro o verde, depois o vermelho, depois o azul. Você continua chamando 
Color(r, g, b) na ordem normal, e a biblioteca reordena antes de enviar. 
Outros chips usam NEO_RGB, NEO_BRG etc., e existem os de 4 cores com LED branco (NEO_GRBW).

NEO_KHZ800: velocidade do sinal. Como é um fio só, sem clock, o chip reconhece cada bit 
pela duração do pulso. O WS2812B trabalha a 800 kHz; chips mais antigos (WS2811) 
usam 400 kHz, que seria NEO_KHZ400.

Se durante a construção do protótipo físico houver alguma cor invertida, aqui é o primeiro 
ponto a se atentar.

- A biblioteca nativa já possui um método setBrightness() mas o brilho como implementado
aqui será delegado pela classe Lampada. Ficar apenas de olho em algum limitador de brilho
caso haja limitação de hardware (o quanto de corrente os leds no brilho máximo consumirão
e o que a placa ESP32 consegue fornecer).


================================================================================
*/


LedRGB::LedRGB(uint8_t pino_din, uint16_t quantidadeLeds)
  : _anel(quantidadeLeds,
          pino_din,
          NEO_GRB + NEO_KHZ800) {} 


bool LedRGB::iniciar(){

  _anel.begin();
  _anel.clear();
  _anel.show();

  // O buffer das cores é alocado no construtor; se faltou memória, vem nullptr
  return _anel.getPixels() != nullptr;

}

void LedRGB::escrever(uint8_t red, uint8_t green, uint8_t blue){
  _anel.fill(Adafruit_NeoPixel::Color(red, green, blue));
  _anel.show();
}

// Apaga a lâmpada settando os valores rgb para 0.
void LedRGB::apagar(){
  escrever(0, 0, 0);
}

