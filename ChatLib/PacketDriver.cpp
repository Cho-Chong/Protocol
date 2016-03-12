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
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

namespace ChatLib
{
    static const int MAX_BUFFER_SIZE = 1024; //TODO: bounded by physical layer
    
    PacketDriver::PacketDriver() : RecBufStart(0),
                                             RecBufEnd(0)
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

    PACKET_STATUS PacketDriver::SendPackets(int socket)
    {
        PACKET_STATUS status = PACKET_SUCCESS;
        int total_bytes_sent = 0;
        
        while(!this->OutgoingPackets[socket].empty() && total_bytes_sent < MAX_BUFFER_SIZE && status == PACKET_SUCCESS)
        {
            PACKET_T packet = this->OutgoingPackets[socket].front();
            this->OutgoingPackets[socket].pop();
            status = SendAll(socket, (char*) &packet, packet.header.length);
            total_bytes_sent += packet.header.length;
        }
        
        return status;
    }
    
    PACKET_STATUS PacketDriver::SendAll(int socket, const char* buf, int length)
    {
        int total = 0;
        int bytes_left = length;
        int bytes_sent = 0;
        
        while(total < bytes_left)
        {
            bytes_sent = send(socket, buf + total, bytes_left, 0);

            if(bytes_sent == -1)
            {
                break;
            }
            else
            {
                total += bytes_sent;
                bytes_left -= bytes_sent;
            }
        }
        
        return bytes_sent == -1 ? PACKET_FAILURE : PACKET_SUCCESS;
    }
    
    PACKET_STATUS PacketDriver::RecvPacket(int socket)
    {
        PACKET_STATUS status = PACKET_SUCCESS;
        bool found_packet = false;
        
        status = Receive(socket, 0);
        
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
                ReceivedPackets.push(packet);
            }
        }
        while(GetRecBufSize() > 0 && found_packet && status != PACKET_FAILURE);
        
        return status;
    }
    
    PACKET_STATUS PacketDriver::Receive(int socket, int timeout)
    {
        PACKET_STATUS status = PACKET_SUCCESS;
        int bytes_received = 0;
        char* cur_buf = &ReceivedBuffer[RecBufEnd];

        if(timeout == 0 && GetRecBufSize() < MAX_RX_BUF_SIZE)
        {
            bytes_received = recv(socket, cur_buf, MAX_BUFFER_SIZE * sizeof(char), 0);
        }
        
        if(bytes_received != -1)
        {
            RecBufEnd += bytes_received;

            if(bytes_received == 0 || GetRecBufSize() >= MAX_RX_BUF_SIZE)
            {
                status = PACKET_FAILURE;
            }
        }
        
        return status;
    }
    
    int PacketDriver::GetRecBufSize()
    {
        return (RecBufEnd > RecBufStart) ? RecBufEnd - RecBufStart : RecBufStart - RecBufEnd;
    }
}
