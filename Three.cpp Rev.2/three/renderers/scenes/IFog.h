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
#include "internal_headers.h"
#include "HasShaderUniforms.h"
#include "Color.h"

namespace three {
    class IFog: public HasShaderUniforms {
    public:
        IFog();
        IFog(Color color);
        virtual ~IFog();
        
        const Color& getColor();
        void setColor(const Color color);
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const IFog& obj ) {
            os << "IFog {" << std::endl;
            os << "\tcolor: " << obj.color << std::endl;
            os << "}";
            return os;
        }
        
    protected:
        Color color;
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__IFog__) */
