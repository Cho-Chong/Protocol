//
//  Packet.hpp
//  ChatLib
//
//  Created by Cho Chong on 2/7/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#ifndef Packet_hpp
#define Packet_hpp

#include <stdio.h>

namespace ChatLib
{
    #define MAX_PACKET_BODY_LENGTH (1024)

    typedef enum
    {
        TYPE_ERROR = 0,
        TYPE_USER,
        TYPE_MESSAGE,
        NUM_PACKET_TYPES
    } PACKET_TYPE_ENUM;
    
    typedef enum
    {
        NODE_ERROR = 0,
        NODE_SERVER,
        NODE_CLIENT,
        NUM_NODE_TYPES
    } PACKET_NODES_ENUM;
    
    struct HEADER_T
    {
        PACKET_TYPE_ENUM type;
        PACKET_NODES_ENUM source;
        PACKET_NODES_ENUM dest;
        unsigned int length;
        unsigned int header_crc;
        unsigned int body_crc;
        
        HEADER_T()
        {
            type = TYPE_ERROR;
            source = NODE_ERROR;
            dest = NODE_ERROR;
            length = 0;
            header_crc = 0;
            body_crc = 0;
        }
    };

    typedef struct
    {
        char buffer[MAX_PACKET_BODY_LENGTH];
    } BODY_T;

    typedef struct
    {
        HEADER_T header;
        BODY_T body;
    } PACKET_T;
    
    #define MAX_PACKET_SIZE (sizeof(PACKET_T))
    #define HEADER_SIZE (sizeof(HEADER_T))
    #define MAX_BODY_SIZE (sizeof(BODY_T))
}

#endif /* Packet_hpp */
