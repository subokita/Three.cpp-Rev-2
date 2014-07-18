//
//  Fog.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Fog__
#define __Three_cpp_Rev_2__Fog__

#include <iostream>
#include "IFog.h"

namespace three {
    class Fog: public IFog {
    public:
        static ptr<Fog> create();
        static ptr<Fog> create( Color color, float near, float far );
        
        Fog();
        Fog( Color color, float near, float far );
        ~Fog();
        
    public:
        float near;
        float far;
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Fog& obj ) {
            os << "Fog {" << std::endl;
            os << "\tnear : " << obj.near  << std::endl;
            os << "\tfar  : " << obj.far   << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__Fog__) */
