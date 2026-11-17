#include "servico_mqtt.h"
#include "certificado_ca.h"
#include "config.h"

ServicoMqtt::ServicoMqtt(InterpretadoraDeEstado& interpretadora)
  : _interpretadora(interpretadora),
    _mqtt(_clienteWiFi),
    _ultimaTentativa(0),
    _estadoPendente(false),
    _ultimaPublicacao(0) {}

// Age como setter do estado pendente 
void ServicoMqtt::marcarEstadoAlterado() {
  _estadoPendente = true;
}

void ServicoMqtt::iniciar() {

  // Define o certificado CA para autenticação
  _clienteWiFi.setCACert(CERTIFICADO_CA);

  _mqtt.setServer(Config::BrokerMQTT::BROKER, Config::BrokerMQTT::PORTA);

  // Aqui há a passagem do método caso uma nova mensagem chegue (callback)
  _mqtt.setCallback(
    [this]
    (char* topico, byte* payload, unsigned int tamanho)
    {                                                    
    aoReceberMensagem(topico, payload, tamanho);
    }
  );
}

void ServicoMqtt::manter() {

  // Se não há conexão Wifi não há porque manter. Já sai do método.
  if(WiFi.status() != WL_CONNECTED){
    return;
  }

  if(!_mqtt.connected()){

    // Adiciona um tempo mínimo de tentativa de reconexão.
    if(millis() - _ultimaTentativa >= INTERVALO_RECONEXAO_MS){

      _ultimaTentativa = millis();
      conectar();

    }
    return;
  }

  // Mantém o loop do MQTT. Continuamente busca por novas mensagens e mantém a conexão ativa.
  _mqtt.loop();

  // Aqui age o Throttle de publicação. Publica o estado pendente a cada intervalo de publicação
  // definido.
  if (_estadoPendente && millis() - _ultimaPublicacao >= INTERVALO_PUBLICACAO_MS) {
    publicarEstado();
  }

}

bool ServicoMqtt::conectar() {

  constexpr uint8_t     QOS_TESTAMENTO   = 1;
  constexpr bool        RETER_MENSAGEM   = true;
  constexpr const char* MENSAGEM_OFFLINE = "{\"online\":false}";
  constexpr const char* MENSAGEM_ONLINE  = "{\"online\":true}";

  Serial.printf("Conectando ao Broker MQTT %s ...\n", Config::BrokerMQTT::BROKER);

  // Ver comentário [1] ao fim do código explicando os parâmetros do método mqtt.connect()

  bool conectou = _mqtt.connect(
                                /* Configurações de autenticação */
                                Config::BrokerMQTT::ID_CLIENTE,
                                Config::BrokerMQTT::USERNAME,
                                Config::BrokerMQTT::PASSWORD,
                                /* Configurações do Last Will*/
                                Config::BrokerMQTT::TOPICO_DISPONIBILIDADE,
                                QOS_TESTAMENTO,
                                RETER_MENSAGEM,
                                MENSAGEM_OFFLINE);

  if(!conectou) {
    Serial.printf("Conexão ao Broker MQTT falhou. Código de erro: %d\n", _mqtt.state());
    return false;
  }

  Serial.println("Conexão ao Broker MQTT realizada com sucesso!");

  // Subscreve e publica nos tópicos apropriados...
  // Avisa ao tópico de disponibilidade que está online
  _mqtt.publish(Config::BrokerMQTT::TOPICO_DISPONIBILIDADE,
                MENSAGEM_ONLINE,
                RETER_MENSAGEM);
  
  // Subscreve para receber comandos (estado requerido para a lâmpada)
  _mqtt.subscribe(Config::BrokerMQTT::TOPICO_CMD);

  publicarEstado();
  
  return true;
}

void ServicoMqtt::publicarEstado() {

  constexpr bool RETER_MENSAGEM = true;

  if(!_mqtt.connected()){
    return;
  }

  String estado = _interpretadora.descreverEstado();


  // O segundo argumento de publish é o payload. O c_str() converte a String para o array de chars
  // que a biblioteca pede (char*)
  _mqtt.publish(Config::BrokerMQTT::TOPICO_STATUS,
                estado.c_str(),
                RETER_MENSAGEM);

  _estadoPendente = false;
  _ultimaPublicacao = millis();

  Serial.println("Estado enviado ao MQTT");
  Serial.println(estado);              
}

void ServicoMqtt::aoReceberMensagem(char* topico, byte* payload, unsigned int tamanho) {

  // O payload NÃO termina em '\0' como o padrão usado em C. Então copiamos usando a variável tamanho...

  String json;

  for (unsigned int i = 0; i < tamanho; i++) {
    json += (char) payload[i];
  }

  Serial.printf("MQTT recebido: %s\n", json.c_str());

  // Envia o json recebido a interpretadora. Se retornar false, o formato JSON está inválido.
  if (!_interpretadora.receberEstado(json)) {
    Serial.println("JSON inválido");
  }
  
  marcarEstadoAlterado();
}



