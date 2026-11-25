#include "repositorio_wifi.h"

bool RepositorioWifi::carregar(String& ssid, String& senha) {
  _preferencias.begin(NAMESPACE, true);   // true = só leitura
  ssid  = _preferencias.getString(CHAVE_SSID, "");   // "" é o valor se a chave não existir
  senha = _preferencias.getString(CHAVE_SENHA, "");
  _preferencias.end();

  return !ssid.isEmpty();
}

void RepositorioWifi::salvar(const String& ssid, const String& senha) {
  _preferencias.begin(NAMESPACE, false);  // false = leitura e escrita
  _preferencias.putString(CHAVE_SSID, ssid);
  _preferencias.putString(CHAVE_SENHA, senha);
  _preferencias.end();
}

void RepositorioWifi::apagar() {
  _preferencias.begin(NAMESPACE, false);
  _preferencias.clear();                  // apaga todas as chaves do namespace
  _preferencias.end();
}