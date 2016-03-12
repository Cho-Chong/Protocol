//
//  PacketWrapper.hpp
//  ChatLib
//
//  Created by Cho Chong on 2/7/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#ifndef PacketWrapper_hpp
#define PacketWrapper_hpp

#include <stdio.h>
#include "Packet.hpp"

namespace ChatLib
{
    class PacketWrapper
    {
    public:
        PacketWrapper();
        ~PacketWrapper();
        
        static void Wrap(const char* buf, int len, PACKET_T& packet, PACKET_TYPE_ENUM type, PACKET_NODES_ENUM dest, PACKET_NODES_ENUM source);
        static void Unwrap(const char* buf,  int len, PACKET_T& packet);
    private:
        static void FindHeader(const char* buf, int len, HEADER_T& header);
    };
}

#endif /* PacketWrapper_hpp */
