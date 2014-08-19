//
//  FogExp2.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "FogExp2.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<FogExp2> FogExp2::create( Color color, float distance ) {
        return make_shared<FogExp2>( FogExp2(color, distance) );
    }
    
    FogExp2::FogExp2( Color color, float distance ) :
        IFog    ( color ),
        distance( distance )
    {}
    
    FogExp2::~FogExp2(){}
    
    
    void FogExp2::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
        shader->setUniform( "fog_color", color, 1.0, gamma );
        shader->setUniform( "fog_density", distance );
    }
    
    float FogExp2::getDistance() {
        return this->distance;
    }
    
    void FogExp2::setDistance( float distance ) {
        this->distance = distance;
    }
}