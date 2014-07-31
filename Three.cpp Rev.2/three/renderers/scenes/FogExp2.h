//
//  FogExp2.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__FogExp2__
#define __Three_cpp_Rev_2__FogExp2__

#include <iostream>
#include "IFog.h"
#include "HasShaderUniforms.h"

namespace three {
    class FogExp2 : public IFog {
    public:
        static ptr<FogExp2> create();
        static ptr<FogExp2> create( Color color, float distance );
        
        virtual void setUniforms( ptr<Shader> shader, bool gamma ) override;
        
        FogExp2();
        FogExp2( Color color, float distance );
        ~FogExp2();
        
        /* Data members */
        float distance;
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const FogExp2& obj ) {
            os << "FogExp2 {" << std::endl;
            os << "\tdistance: " << obj.distance << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__FogExp2__) */
