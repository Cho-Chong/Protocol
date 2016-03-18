//
//  TcpDevice
//  ChatLib
//
//  Created by Cho Chong on 3/15/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#ifndef TCPDEVICE_H
#define TCPDEVICE_H

#include "IIODevice.h"
#include <sys/types.h>
//TODO: move to cpp
namespace ChatLib
{
    
    #define MAX_INFO_LEN (128)
    #define MAX_CONNECTIONS (10)
    
    typedef enum
    {
        TCP_SERVER = 0,
        TCP_CLIENT,
    } TCP_TYPE;
    
    typedef struct
    {
        TCP_TYPE Type;
        SINT InputSocket;
        fd_set SocketSet;
        char Port[MAX_INFO_LEN];
        SINT MaxSocket;
        char Host[MAX_INFO_LEN];
    } TCP_CONNECTION;
    
    class TcpDevice : public Interface::IIODevice
    {
    public:
        TcpDevice() {}
        ~TcpDevice() { Disconnect(); }
        
        STATUS Connect();
        
        STATUS Listen();
        
        STATUS Disconnect();
        
        virtual SINT Read(SINT fd, BYTE* buffer, UINT byte_size) const;
        
        virtual SINT Write(SINT fd, const BYTE* buffer, UINT byte_size) const;
        
        
    private:
        TCP_CONNECTION Connection;
    };
}


#endif /* TCPDEVICE_H */
