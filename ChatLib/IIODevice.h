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
            IIODevice() {}
            ~IIODevice() {}
            
            virtual SINT Read(SINT fd, BYTE* buffer, UINT byte_size) const = 0;
            
            virtual SINT Write(SINT fd, const BYTE* buffer, UINT byte_size) const = 0;
        };
    }
}


#endif /* IIODevice_h */
