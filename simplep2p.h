#ifndef SIMPLE_P2P
#define SIMPLE_P2P

#include <enet/enet.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "reference.h"

class SimpleP2P : public Reference{
    OBJ_TYPE(SimpleP2P,Reference);

protected:
    static void _bind_methods();

private:

    ENetAddress address;
    ENetEvent event;
    ENetHost *host;
    ENetPeer *peer;

    bool statusConnection;
    int unique_id;

    /*!
       \brief Gera um id.
       \param int grain. Max = 999999999. Padrão = 1000000000
       \return int unique_id
    */
    int SimpleP2P_Generate_unique_id(int grain=1000000000);

public:

    int _clients=2;
    int _channels=2;

    /*! Construtor */
    SimpleP2P();

    /*!
       \brief Inicializa o Enet.
       \param void
       \return int = 0 Sucesso
               int < 0 Falha
    */
    int SimpleP2P_init(void);

    /*!
       \brief Desliga o Enet.
       \param void
       \return void
    */
    void SimpleP2P_deinit(void);

    /*!
       \brief Para Criar um host.
       \param String _address Endereço do host. "0" = Vai para o endereço local.
              int _port Porta a se conectar. Padrão = 8040
              int _clients Quantidade de nós que pode se conectar. Padrão = 2
              int _channels Quantidade de canais. Padrão = 2. Se 0 é igual a ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT(256)
              int _incoming_bandwidth Largura de banda recebida. Padrão =  0(A ENet assumirá a largura de banda ilimitada)
              int _outgoing_bandwidth Largura de banda passada. Padrão = 0(A ENet assumirá a largura de banda ilimitada)
       \return int = 0 Sucesso
               int < 0 Falha
    */
    int SimpleP2P_CreateHost(
        String _address,
        int _port=8040,
        int _incoming_bandwidth=0,
        int _outgoing_bandwidth=0,
        int _grain=1000000000
    );

    /*!
       \brief Para destruir um host.
       \param void
       \return void
    */
    void SimpleP2P_DestroyHost(void);

    /*!
       \brief "Description"
       \param  int _timeout Tempo em milesegundos para verificar os eventos. Se 0(Não bloqueado). Padrão = 1
       \return > 0 Se o evento ocorreu dentro do tempo especificado
               0 Se não ocorreu evento
               < 0 Se falhou
    */
    void SimpleP2P_LoopService(int _timeout=1);

    /*!
       \brief Para conectar com outro host.
       \param String _address Endereço de destino a se conectar.
              int _channels Quantidade de canais. Padrão = 2
       \return int = 0 Sucesso
               int = -1 Falha
    */
    int SimpleP2P_Connect(String _address, int _channels=2);

    /*!
       \brief Função para desconectar um peer.
       \param ENetPeer *peer O peer a se desconectar.
       \return void
    */
    void SimpleP2P_Disconnect(void);

    /*!
       \brief Função para enviar um pacote.
       \param const char *_packet Dados a enviar.
              bool _reliable Se true confiavel(Reenvia caso tenha se perdido). Se false não confiavel(Não reenvia).Padrão = true
              bool _broadcast Se true envia para todos os peers conectados. Padrão = true
              int _channel O canal pelo qual irá enviar o pacote. Padrão = 0
              ENetPeer *_peer=NULL O peer para qual irá enviar o pacote ao destino(Somente quando _broadcast = false). Padrão = NULL
       \return int = 0 Sucesso
               int < 0 Falha
    */
    int SimpleP2P_SendPacket(String _packet, bool _realiable=true, bool _broadcast=true, int _channel=0);


};

#endif
