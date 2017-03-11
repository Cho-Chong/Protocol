/*
 *  PacketDriver.hpp
 *  PacketDriver
 *
 *  Created by Cho Chong on 2/7/16.
 *  Copyright © 2016 Cho Chong. All rights reserved.
 *
 */

#ifndef PacketDriver_
#define PacketDriver_

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "Packet.hpp"
#include <queue>
#include <map>
#include "IIODevice.h"

namespace ChatLib
{
    typedef enum
    {
        PACKET_SUCCESS = 0,
        PACKET_FAILURE,
    } PACKET_STATUS;
    
    #define MAX_RX_BUF_SIZE (4096)

    class PacketDriver
    {
    public:
        PacketDriver(Interface::IIODevice* iodevice);
        ~PacketDriver();
        
        void QueuePacket(int socket, const PACKET_T& packet);
        void DequeuePacket(PACKET_T& packet);
        
        PACKET_STATUS SendPackets(int socket);
        PACKET_STATUS RecvPacket(int socket);
        void CleanSocket(int socket);
    private:
        
        PACKET_STATUS Receive(int socket, int timeout);
        
        PACKET_STATUS SendAll(int socket, const char* buf, int length);
        int GetRecBufSize();
        
        //TODO: use a packet pool to get rid of allocation in queues
        std::map<int, std::queue<PACKET_T>> OutgoingPackets;
        
        //circ buffer
        char ReceivedBuffer[MAX_RX_BUF_SIZE];
        int RecBufSize;
        int RecBufStart;
        int RecBufEnd;
        std::queue<PACKET_T> ReceivedPackets;
        Interface::IIODevice* Device;
    };
}

#pragma GCC visibility pop
#endif
