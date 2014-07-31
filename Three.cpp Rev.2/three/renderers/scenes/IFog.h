//
//  IFog.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__IFog__
#define __Three_cpp_Rev_2__IFog__

#include <iostream>
#include "Color.h"
#include "Constants.h"
#include "HasShaderUniforms.h"

namespace three {
    class IFog: public HasShaderUniforms {
    public:
        IFog();
        IFog(Color color);
        virtual ~IFog();
        
    public:
        Color color;
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const IFog& obj ) {
            os << "IFog {" << std::endl;
            os << "\tcolor: " << obj.color << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__IFog__) */
