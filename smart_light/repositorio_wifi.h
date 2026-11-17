#pragma once
#include <Arduino.h>
#include <Preferences.h>

/*
  Classe que armazena as credenciais da rede WiFi obtidas através do pareamento Bluetooth.

  As credenciais ficam salvas na NVS (non volatile storage) da placa, permitindo que a lâmpada
  se reconecte mesmo após sofrer uma queda de energia / reinicialização inesperada, dispensando
  a necessidade de provisionar as credenciais novamente por bluetooth...

  Quem lida com isso é a biblioteca Preferences.h

  Ele organiza o storage em namespaces, e os dados no padrão chave-valor.
*/
class RepositorioWifi {
  public:
    
    bool carregar(String& ssid, String& senha);

    void salvar(const String& ssid, const String& senha);

    void apagar();

  private:
    Preferences _preferencias;

    static constexpr const char* NAMESPACE   = "wifi";
    static constexpr const char* CHAVE_SSID  = "ssid";
    static constexpr const char* CHAVE_SENHA = "senha";
};