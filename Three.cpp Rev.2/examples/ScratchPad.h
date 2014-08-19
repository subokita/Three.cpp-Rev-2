//
//  ScratchPad.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__ScratchPad__
#define __Three_cpp_Rev_2__ScratchPad__

#include <iostream>

#include "Example.h"

namespace three {
    
    /**
     * ScratchPad is a ... scratch pad, it lets me try out ideas or things that I want to implement next
     * but not making it an official effort yet
     */
    class ScratchPad: public Example {
    public:
        static ptr<ScratchPad> create();
        
        virtual void run() override;
    };
}
#endif /* defined(__Three_cpp_Rev_2__ScratchPad__) */
