//
//  PacketWrapper.cpp
//  ChatLib
//
//  Created by Cho Chong on 2/7/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#include "PacketWrapper.hpp"
#include "Crc.hpp"
#include <string.h>
#include <stdio.h>
#include <iostream>

namespace ChatLib
{
    PacketWrapper::PacketWrapper()
    {
        
    }
    
    PacketWrapper::~PacketWrapper()
    {
        
    }
    
    void PacketWrapper::FindHeader(const char* buf, int len, HEADER_T& header)
    {
        if(len >= HEADER_SIZE && Crc::CalcCrc(buf, HEADER_SIZE) == ((HEADER_T *)buf)->header_crc)
        {
            header = *((HEADER_T *)buf);
        }
    }
    
    //CHO_ACTIVE: what if len is greater than a max packet size?
    void PacketWrapper::Wrap(const char* buf, int len, PACKET_T& packet, PACKET_TYPE_ENUM type, PACKET_NODES_ENUM dest, PACKET_NODES_ENUM source)
    {
        packet.header.source = source;
        packet.header.dest = dest;
        packet.header.length = HEADER_SIZE + len;
        packet.header.type = type;
        packet.header.body_crc = Crc::CalcCrc(buf, len);
        packet.header.header_crc = Crc::CalcCrc((char*) &packet.header, HEADER_SIZE);
        
        memcpy(packet.body.buffer, buf, len);
    }
    
    void PacketWrapper::Unwrap(const char* buf, int len, PACKET_T& packet)
    {
        PACKET_T result;
        std::memset(&packet, 0, MAX_PACKET_SIZE);
        
        FindHeader(buf, len, result.header);
        
        if(result.header.length > 0)
        {
            int body_len = result.header.length - HEADER_SIZE;
            int remaining_bytes = len - HEADER_SIZE;
            
            if(remaining_bytes >= body_len && Crc::CalcCrc(buf + HEADER_SIZE, body_len) == result.header.body_crc )
            {
                packet.header = result.header;
                packet.body = *((BODY_T*) &buf[HEADER_SIZE]);
            }
        }
    }
}