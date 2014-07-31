//
//  FogExp2.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "FogExp2.h"
#include "three.h"

using namespace std;
namespace three {
    
    ptr<FogExp2> FogExp2::create() {
        return make_shared<FogExp2>();
    }
    
    ptr<FogExp2> FogExp2::create( Color color, float distance ) {
        return make_shared<FogExp2>( FogExp2(color, distance) );
    }
    
    FogExp2::FogExp2():
        distance( 0.00025 )
    {}
    
    FogExp2::FogExp2( Color color, float distance ) :
        IFog    ( color ),
        distance( distance )
    {}
    
    FogExp2::~FogExp2(){}
    
    
    void FogExp2::setUniforms( ptr<Shader> shader, bool gamma ) {
        shader->setUniform( "fog_color", color, 1.0, gamma );
        shader->setUniform( "fog_density", distance );
    }
}