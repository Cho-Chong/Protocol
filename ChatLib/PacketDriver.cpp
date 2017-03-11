/*
 *  PacketDriver.cpp
 *  PacketDriver
 *
 *  Created by Cho Chong on 2/7/16.
 *  Copyright © 2016 Cho Chong. All rights reserved.
 *
 */

#include <iostream>
#include "PacketDriver.hpp"
#include "PacketWrapper.hpp"

#include <string.h>

namespace ChatLib
{
    
    PacketDriver::PacketDriver(Interface::IIODevice* iodevice) : RecBufStart(0),
                                             RecBufEnd(0),
                                             Device(iodevice)
    {
    }
    
    PacketDriver::~PacketDriver()
    {
        
    }
    
    void PacketDriver::CleanSocket(int socket)
    {
        this->OutgoingPackets.erase(socket);
    }
    
    void PacketDriver::QueuePacket(int socket, const PACKET_T& packet)
    {
        this->OutgoingPackets[socket].push(packet);
    }
    
    void PacketDriver::DequeuePacket(PACKET_T& packet)
    {
        if(!this->ReceivedPackets.empty())
        {
            packet = this->ReceivedPackets.front();
            this->ReceivedPackets.pop();
        }
        else
        {
            std::memset(&packet, 0, MAX_PACKET_SIZE);
        }
    }

    //TODO: get rid of packet status and merge into errors.h
    PACKET_STATUS PacketDriver::SendPackets(int socket)
    {
        STATUS status = STATUS_OK;
        int total_bytes_sent = 0;
        
        while(!this->OutgoingPackets[socket].empty() && total_bytes_sent < MAX_PACKET_SIZE && status == STATUS_OK)
        {
            auto packet = this->OutgoingPackets[socket].front();
            this->OutgoingPackets[socket].pop();
            auto bytes_sent = Device->Write(socket, (BYTE*) &packet, packet.header.length);
            if(bytes_sent != packet.header.length)
            {
                status = PACKET_FAILURE;
            }
            total_bytes_sent += bytes_sent;
        }
        
        return status == STATUS_OK ? PACKET_SUCCESS : PACKET_FAILURE;
    }

    PACKET_STATUS PacketDriver::RecvPacket(int socket)
    {
        PACKET_STATUS status = PACKET_SUCCESS;
        bool found_packet = false;
        
        auto bytes_read = Device->Read(socket, &ReceivedBuffer[RecBufStart], MAX_PACKET_SIZE);
        
        if(bytes_read > 0)
        {
            RecBufEnd += bytes_read;
        }
        else
        {
            //TODO: socket bad. need to notify
            status = PACKET_FAILURE;
        }
        
        do
        {
            PACKET_T packet;
            PacketWrapper::Unwrap(&ReceivedBuffer[RecBufStart], GetRecBufSize(), packet);
            RecBufStart += packet.header.length;
            found_packet = packet.header.length > 0;
            
            if(GetRecBufSize() > MAX_PACKET_SIZE && !found_packet)
            {
                //if we can't find any good header or body, then throw out these bytes
                RecBufStart += MAX_PACKET_SIZE;
                status = PACKET_FAILURE;
            }
            else if (found_packet)
            {
                //TODO: push to the router
                ReceivedPackets.push(packet);
            }
        }
        while(GetRecBufSize() > 0 && found_packet && status != PACKET_FAILURE);
        
        return status;
    }
    
    
    int PacketDriver::GetRecBufSize()
    {
        return (RecBufEnd > RecBufStart) ? RecBufEnd - RecBufStart : RecBufStart - RecBufEnd;
    }
}
