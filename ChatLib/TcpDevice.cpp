//
//  TcpDevice.cpp
//  ChatLib
//
//  Created by Cho Chong on 3/16/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#include "TcpDevice.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

namespace ChatLib
{
    STATUS TcpDevice::Connect()
    {
        int sockfd = 0;
        struct addrinfo info, *servinfo, *cur_sock;
        int yes = 1;
        int rv;
        
        memset(&info, 0, sizeof(info));
        info.ai_family = AF_UNSPEC;
        info.ai_socktype = SOCK_STREAM;
        info.ai_flags = AI_PASSIVE;
        
        //FD_ZERO(&Connection.SocketSet);
        
        if ( (rv = getaddrinfo(NULL, Connection.Port, &info, &servinfo) != 0))
        {
            return STATUS_ERROR;
        }
        else
        {
            for(cur_sock = servinfo; cur_sock != NULL; cur_sock = cur_sock->ai_next)
            {
                if( (sockfd = socket(cur_sock->ai_family, cur_sock->ai_socktype, cur_sock->ai_protocol)) == -1)
                {
                    continue;
                }
                
                if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(int)) == -1)
                {
                    return 0;
                }
                
                if(bind(sockfd, cur_sock->ai_addr, cur_sock->ai_addrlen) == -1)
                {
                    close(sockfd);
                    continue;
                }
                
                break;
            }
            
            freeaddrinfo(servinfo);
            
            if(cur_sock == NULL)
            {
                return STATUS_ERROR;
            }
            
//            if(listen(sockfd, MAX_PENDING_CONNECTIONS) == -1)
//            {
//                perror("listen");
//                return 0;
//            }
//            else
//            {
//                FD_SET(sockfd, &Connection.SocketSet);
//            }
            
            Connection.InputSocket = sockfd;
            Connection.MaxSocket = sockfd;
            
            return STATUS_OK;
        }
    }
    
    STATUS TcpDevice::Disconnect()
    {
        return STATUS_ERROR;
    }
    
    STATUS TcpDevice::Listen()
    {
        return STATUS_ERROR;
    }
    
    STATUS TcpDevice::Read(SINT fd, BYTE* buffer, UINT byte_size) const
    {
        SINT bytes_read = 0;
        UINT total_bytes = 0;
        
        do
        {
            bytes_read = recv(Connection.InputSocket, &buffer[total_bytes], byte_size - total_bytes, 0);
            
            if(bytes_read > 0)
            {
                total_bytes += bytes_read;
            }
        }while( total_bytes < byte_size && bytes_read > 0);
        
        return total_bytes == byte_size ? STATUS_OK : STATUS_ERROR;
    }
    
    STATUS TcpDevice::Write(SINT fd, const BYTE* buffer, UINT byte_size) const
    {
        SINT bytes_written = 0;
        UINT total_bytes = byte_size;
        
        do
        {
            bytes_written = send(Connection.InputSocket, &buffer[byte_size - total_bytes], total_bytes, 0);
            
            if(bytes_written > 0)
            {
                total_bytes -= bytes_written;
            }
        }while ( total_bytes > 0 && bytes_written > 0);
        
        return total_bytes == 0 ? STATUS_OK : STATUS_ERROR;
    }

}