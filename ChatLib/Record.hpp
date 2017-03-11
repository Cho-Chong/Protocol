//
//  Record.hpp
//  ChatLib
//
//  Created by Cho Chong on 2/10/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#ifndef Record_hpp
#define Record_hpp

#include <stdio.h>
#include <string>
#include "Packet.hpp"

typedef unsigned int ID_T;

typedef enum
{
    TIME_LINUX,
    TIME_WINDOWS
} TIME_TYPE;

typedef struct
{
    unsigned int Ticks;
    TIME_TYPE Type;
} TIMESTAMP_T;

namespace Model
{

    class Record
    {
    public:
        Record() {};
        ~Record() {};
        
        ID_T GetId() const { return Id; }
        void SetId(const ID_T& id) { Id = id; }
        
        ChatLib::PACKET_TYPE_ENUM GetType() const { return Type; }
    protected:
        ID_T Id;
        ChatLib::PACKET_TYPE_ENUM Type;
    };

#define MAX_ENTRY_SIZE (100)
    
    class Message : public Record
    {
    public:
        Message() { Type = ChatLib::TYPE_MESSAGE; }
        ~Message() {}
        
        ID_T GetUserId() const { return UserId; }
        void SetUserId(const ID_T& id) { UserId = id; }
        char Entry[MAX_ENTRY_SIZE];
        int  GetTime() const { return Timestamp.Ticks; }
        void SetTime(const int &ticks) {Timestamp.Ticks = ticks;}
    private:
        TIMESTAMP_T Timestamp;
        ID_T UserId;
    };
    
#define MAX_NAME_SIZE (20)
    
    class User : public Record
    {
    public:
        User() { Type = ChatLib::TYPE_USER; }
        ~User() {}
        
        char Name[MAX_NAME_SIZE];
    private:
        
    };
}

#endif /* Record_hpp */
