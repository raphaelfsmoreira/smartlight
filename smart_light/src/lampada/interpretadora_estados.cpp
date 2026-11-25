/* 
  A Classe Interpretadora de estados funciona como uma classe
  de camada interpretadora.
  
  Ela serve tanto para receber os JSONs de estado da lampada via MQTT e
  aplicar os estados novos

  COMO TAMBÉM
  
  capturar o estado atual da lâmpada e convertê-lo para JSON
  para envia-lo ao broker MQTT.
  

*/

#include "interpretadora_estados.h"

// Inclui a biblioteca de manejamento de jsons.
// Documentos json irá ser padrão do payload das mensagens MQTT.
#include <ArduinoJson.h>

InterpretadoraDeEstado::InterpretadoraDeEstado(Lampada& lampada)
  : _lampada(lampada) {}

bool InterpretadoraDeEstado::receberEstado(const String& json) {
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, json);

  // JSON quebrado ou que não é um objeto {...}
  if (erro || !doc.is<JsonObject>()) {
    return false;
  }

  // "ligado": liga ou desliga
  if (doc["ligado"].is<bool>()) {
    if (doc["ligado"].as<bool>()) {
      _lampada.ligar();
    } else {
      _lampada.desligar();
    }
  }

  // "brilho": absoluto, em %
  if (doc["brilho"].is<int>()) {
    _lampada.definirBrilho(doc["brilho"].as<int>());
  }

  // "rgb": lista com 3 valores de 0 a 255
  if (doc["rgb"].is<JsonArray>()) {
    JsonArray rgb = doc["rgb"].as<JsonArray>();

    if (rgb.size() == 3) {
      int red   = constrain(rgb[0].as<int>(), 0, 255);
      int green = constrain(rgb[1].as<int>(), 0, 255);
      int blue  = constrain(rgb[2].as<int>(), 0, 255);
      _lampada.definirCor(red, green, blue);
    }
  }

  return true;
}

String InterpretadoraDeEstado::descreverEstado() const {

  JsonDocument doc;
  doc["ligado"] = _lampada.estaLigada();
  doc["brilho"] = _lampada.getBrilho();

  JsonArray rgb = doc["rgb"].to<JsonArray>();
  rgb.add(_lampada.getRed());
  rgb.add(_lampada.getGreen());
  rgb.add(_lampada.getBlue());

  String saida;
  serializeJson(doc, saida);
  return saida;
}