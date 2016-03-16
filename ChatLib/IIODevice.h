//
//  IIODevice.h
//  ChatLib
//
//  Created by Cho Chong on 3/15/16.
//  Copyright © 2016 Cho Chong. All rights reserved.
//

#ifndef IIODevice_h
#define IIODevice_h

#include "TypeDefs.h"
#include "Errors.h"

namespace ChatLib
{
    namespace Interface
    {
        class IIODevice
        {
        public:
            IIODevice();
            ~IIODevice();
            
            // Each IO device must try to read/write to given byte size and return error if fail
            virtual STATUS Read(SINT fd, BYTE* buffer, UINT byte_size) const = 0;
            
            virtual STATUS Write(SINT fd, const BYTE* buffer, UINT byte_size) const = 0;
        };
    }
}


#endif /* IIODevice_h */
