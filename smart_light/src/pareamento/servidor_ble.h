#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>

/*
  Servidor BLE do pareamento. Anuncia "SmartLight-XXXX" e expõe 3 características:
  - info   (READ):   modelo e firmware da lâmpada
  - config (WRITE):  o app escreve o JSON com a rede
  - status (NOTIFY): a lâmpada avisa o app como está indo

  Os callbacks da NimBLE rodam na tarefa do BLE, não no loop(). Por isso o
  onWrite só guarda o JSON e levanta uma flag; quem processa é o loop().
*/

class ServidorBle : public NimBLEServerCallbacks,
                    public NimBLECharacteristicCallbacks {
  public:
    void iniciar();   // liga o BLE e começa a anunciar
    void parar();     // desliga o BLE e libera a memória

    bool estaAtivo() const;
    bool estaConectado() const;

    // true quando o app escreveu uma config que o loop ainda não leu
    bool temConfigNova() const;

    // Devolve a config recebida e abaixa a flag
    String lerConfig();

    // Envia um aviso pro app pela característica status
    void notificarStatus(const char* status);

    // ---- Callbacks da NimBLE (sobrescritos) ----
    void onConnect(NimBLEServer* servidor, NimBLEConnInfo& conexao) override;
    void onDisconnect(NimBLEServer* servidor, NimBLEConnInfo& conexao, int motivo) override;
    void onWrite(NimBLECharacteristic* caracteristica, NimBLEConnInfo& conexao) override;

  private:
    NimBLECharacteristic* _status = nullptr;
    bool _ativo = false;

    // Escritos pela tarefa do BLE, lidos pelo loop()
    volatile bool _conectado = false;
    volatile bool _configPendente = false;
    String _config;
};