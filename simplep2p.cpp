#include "simplep2p.h"

SimpleP2P::SimpleP2P()
{
    SimpleP2P_init();
}

int SimpleP2P::SimpleP2P_init(void)
{
    if (enet_initialize () != 0)
    {
        fprintf (stderr,"An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    return 0;
}

void SimpleP2P::SimpleP2P_deinit(void)
{
    atexit (enet_deinitialize);
}

int SimpleP2P::SimpleP2P_CreateHost(String _address, int _port, int _incoming_bandwidth, int _outgoing_bandwidth, int _grain)
{
    //char *my_char = new char[_address.length()+1];
    //memcpy(my_char, _address.c_str(), _address.length() + 1);
    if(strcmp(_address.ascii(), "0") == 0)
    {
        address.host = ENET_HOST_ANY;
    }
    else
    {
        enet_address_set_host (&address, _address.ascii());
        printf("Cheguei aqui! \n");
    }

    address.port = _port;

    host = enet_host_create (&address, _clients, _channels, _incoming_bandwidth,_outgoing_bandwidth);

    if (host == NULL)
    {
        fprintf (stderr, "An error occurred while trying to create an ENet host.\n");
        exit (EXIT_FAILURE);
        return -1;
    }
    unique_id = SimpleP2P_Generate_unique_id(_grain);
    printf("Unique_id %d \n", unique_id);
    return 0;
}

void SimpleP2P::SimpleP2P_DestroyHost(void)
{
    enet_host_destroy(host);
}

void SimpleP2P::SimpleP2P_LoopService(int _timeout)
{
    while(enet_host_service(host, &event, _timeout) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                printf ("A new client connected from %x:%u.\n",
                        event.peer -> address.host,
                        event.peer -> address.port);
                        /* Armazene qualquer informação relevante do cliente aqui. */
                        event.peer -> data = &unique_id;
                        statusConnection = true;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                printf ("A packet of length %zu containing %s was received from %s on channel %u.\n",
                        event.packet -> dataLength,
                        event.packet -> data,
                        event.peer -> data,
                        event.channelID);

                /* Limpe o pacote agora que acabamos de usá-lo. */
                enet_packet_destroy (event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf ("%s disconnected.\n", event.peer -> data);
                /* Redefinir as informações do host. */
                event.peer -> data = NULL;
                statusConnection = false;
                break;

            case ENET_EVENT_TYPE_NONE:
                /* Não ocorreu evento algum. */
                break;
        }
    }
}

int SimpleP2P::SimpleP2P_Generate_unique_id(int _grain)
{
    int _id = 0;

    srand((unsigned)time(NULL));
    if(_grain > 0)
    {
        _id = rand() * _grain;
    }

    return _id = rand();
}

int SimpleP2P::SimpleP2P_Connect(String _address, int _channels){

    ENetAddress destination;
    enet_address_set_host (&destination, _address.ascii());

    /*Iniciar conexão com o _host fornecido. */
    peer = enet_host_connect (host, &destination, _channels, unique_id);
    if (peer == NULL)
    {
       fprintf (stderr, "No available peers for initiating an ENet connection.\n");
       exit (EXIT_FAILURE);
       return -1;
    }
    /* Aguarde até 5 segundos para a tentativa de conexão ter sucesso. */
    if (enet_host_service (host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts ("Connection succeeded.");

    }
    else
    {
        /* ou os 5 segundos foram ativados ou um evento de desconexão foi recebido.
        atualizar o peer no caso de os 5 segundos se esgotarem sem nenhum evento significativo.  */
        enet_peer_reset (peer);
        puts ("Connection failed.");
        return -1;
    }
    return 0;
}

void SimpleP2P::SimpleP2P_Disconnect(void)
{
    enet_peer_disconnect (peer, unique_id);

    /* Aguarde até 3 segundos para a desconexão ter sucesso. */
    if(enet_host_service (host, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_DISCONNECT)
    {
        puts ("Disconnection succeeded.");
    }
    /* Se a tentativa de desconectar falhar. Forçar a conexão para baixo. */
    enet_peer_reset (peer);
}

int SimpleP2P::SimpleP2P_SendPacket(String _packet, bool _realiable, bool _broadcast, int _channel)
{
    ENetPacket * packet;

    /* Criar o pacote confiavel." */
    if(_realiable)
    {
        packet = enet_packet_create (_packet.ascii(), strlen (_packet.ascii()) + 1, ENET_PACKET_FLAG_RELIABLE);
    }
    /* Criar o pacote não confiavel(Obs: Pode exercer ao MTU que será fragmentado)." */
    else
    {
        packet = enet_packet_create (_packet.ascii(), strlen (_packet.ascii()) + 1, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    }

    /* Enviar o pacote para todos os peers conectados ao host. */
    if(_broadcast)
    {
        enet_host_broadcast	(host, _channel, packet);
    }
    /* Enviar o pacote para o destino do peer conectado. */
    else
    {
        return -1;
        //return enet_peer_send (_peer, _channel, packet);
    }
    return 0;
}

void SimpleP2P::_bind_methods() {

    ObjectTypeDB::bind_method("SimpleP2P_init",&SimpleP2P::SimpleP2P_init);
    ObjectTypeDB::bind_method("SimpleP2P_deinit",&SimpleP2P::SimpleP2P_deinit);
    ObjectTypeDB::bind_method("SimpleP2P_CreateHost",&SimpleP2P::SimpleP2P_CreateHost);
    ObjectTypeDB::bind_method("SimpleP2P_DestroyHost",&SimpleP2P::SimpleP2P_DestroyHost);
    ObjectTypeDB::bind_method("SimpleP2P_LoopService",&SimpleP2P::SimpleP2P_LoopService);
    ObjectTypeDB::bind_method("SimpleP2P_Connect",&SimpleP2P::SimpleP2P_Connect);
    ObjectTypeDB::bind_method("SimpleP2P_Disconnect",&SimpleP2P::SimpleP2P_Disconnect);
    ObjectTypeDB::bind_method("SimpleP2P_SendPacket",&SimpleP2P::SimpleP2P_SendPacket);
}
