//
//  PacketRouter.hpp
//  ChatLib
//
//  Created by Cho Chong on 2/12/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#ifndef PacketRouter_hpp
#define PacketRouter_hpp

#include <stdio.h>
#include "Packet.hpp"
#include <map>
#include <vector>

namespace Service
{
    // will dequeue packets and route them to the correct subscriber (other nodes or our database)
    // TODO: could i have the same centralized database paradigm in c++
    //typedef (void*)(ChatLib::PACKET_T) PACKET_CALLBACK;
    typedef unsigned int PACKET_CALLBACK;
    
    class PacketRouter
    {
    public:
        PacketRouter() {}
        ~PacketRouter() {}
        
        void Process();  // process in/out going packets
        void Subscribe(ChatLib::PACKET_TYPE_ENUM type, PACKET_CALLBACK callback); // subscribe a callback to a packet type
    private:
        void Route(); // given a packet type, routes to a subscribed callback
        std::map<ChatLib::PACKET_TYPE_ENUM, std::vector<PACKET_CALLBACK>> SubscriberMap;
    };
}

#endif /* PacketRouter_hpp */
