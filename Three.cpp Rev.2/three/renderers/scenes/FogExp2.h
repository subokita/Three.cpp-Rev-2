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
        static ptr<FogExp2> create( Color color, float distance = 0.00025 );
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        ~FogExp2();
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const FogExp2& obj ) {
            os << "FogExp2 {" << std::endl;
            os << "\tdistance: " << obj.distance << std::endl;
            os << "}";
            return os;
        }
        
        float getDistance();
        void setDistance( float distance );
        
    protected:
        FogExp2( Color color, float distance );
        
    protected:
        float distance;
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__FogExp2__) */
