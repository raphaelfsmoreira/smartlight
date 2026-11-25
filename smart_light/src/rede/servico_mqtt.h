#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "../lampada/interpretadora_estados.h"

class ServicoMqtt{
  public:
    ServicoMqtt(InterpretadoraDeEstado& interpretadora);

    void iniciar();

    // Chamado no loop. Reconecta se cair e processa mensagens que chegam.
    void manter();

    void publicarEstado();

    void marcarEstadoAlterado();

  private:
    InterpretadoraDeEstado& _interpretadora;
    WiFiClientSecure _clienteWiFi;
    PubSubClient _mqtt;
    
    // --------------------------------------------------------------------------------
    // Variáveis para tentativa de reconexão
    // --------------------------------------------------------------------------------

    // Intervalo de espera para reconectar caso haja queda na conexão ao broker
    static constexpr unsigned long INTERVALO_RECONEXAO_MS = 5000;
    unsigned long _ultimaTentativa;

    // --------------------------------------------------------------------------------
    // Variáveis de controle do Throttle de publicação
    // --------------------------------------------------------------------------------

    // O throttle evita que cada mudança em um intervalo de tempo curto (como girar o encoder)
    // gere uma nova publicação no mqtt.

    // Limite de publicação: mudanças de estado são agrupadas e publicadas
    // no máximo uma vez a cada INTERVALO_PUBLICACAO_MS
    static constexpr unsigned long INTERVALO_PUBLICACAO_MS = 500;
    bool _estadoPendente;
    unsigned long _ultimaPublicacao;


    /* 
      Método da "cerimônia" de entrada ao broker MQTT.
      - Conecta, deixando o Last Will (testamento), isto é, uma mensagem pronta caso a conexão ao broker
      for perdida. No caso, o Last Will será o status "offline" no tópico de Availability.
      - Ao conectar, publica que está online no mesmo tópico.
      - Assina no tópico de comandos (cmd) para poder recebè-los.
      - Publica o estado atual da lâmpada para quem estiver subscribed poder obter o estado (no caso, o app).
    */ 
    bool conectar();

    /*
      É o callback chamado pela biblioteca PubSubClient. Esse método nunca é chamado diretamente.
      A cada volta do MQTT loop há uma espiada em busca de uma nova mensagem. Essa função é passada
      no setCallBack e registrada. Toda vez que uma nova mensagem chega, o loop chama este método.
    */
    void aoReceberMensagem(char* topico, byte* payload, unsigned int tamanho);

};