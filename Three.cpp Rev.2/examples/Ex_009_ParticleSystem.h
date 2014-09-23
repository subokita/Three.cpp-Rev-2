//
//  Ex_009_ParticleSystem.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_009_ParticleSystem__
#define __Three_cpp_Rev_2__Ex_009_ParticleSystem__

#include <iostream>
#include "Example.h"

namespace three{
    /**
     * Example on how to use particle system
     * It's still very much a prototype, please refrain from using it
     */
    class Ex_009_ParticleSystem: public Example {
    public:
        static ptr<Ex_009_ParticleSystem> create();
        
        virtual void run() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ex_009_ParticleSystem__) */
