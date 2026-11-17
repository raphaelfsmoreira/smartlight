#pragma once
#include <Arduino.h>
#include "repositorio_wifi.h"

class ConexaoWifi {
  public:
    ConexaoWifi();

    // Registra o aviso de desconexão e conecta na rede salva, se houver.
    // Sem rede salva, fica em modo offline.
    void iniciar();

    // Rede nova (console, depois pareamento): salva e conecta.
    void trocarRede(const String& ssid, const String& senha);

    // Apaga a rede salva e desliga o rádio (modo offline).
    void esquecer();

    // Chamado no loop. Avisa quedas e reconecta. Sem rede cadastrada, não faz nada.
    void manter();

    bool estaConectada() const;

  private:
    RepositorioWifi _repositorio;   // composição: construído sozinho
    String _ssid;                   // cópia em RAM do que está salvo
    String _senha;
    bool _estavaConectada;
    unsigned long _ultimaTentativa;

    // Escrito pelo evento do Wi-Fi, lido pelo manter()
    volatile uint8_t _motivoDesconexao;

    void conectar();                // agora privado e sem parâmetros
    void tentarConectar();
    static const char* descreverMotivo(uint8_t motivo);
};