//
//  AmbientLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "AmbientLight.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<AmbientLight> AmbientLight::create(Color color) {
        return make_shared<AmbientLight>( AmbientLight(color) );
    }
    
    /** Constructors **/
    AmbientLight::AmbientLight()
    {}
    
    AmbientLight::AmbientLight(Color& color) :
        Light(color, 1.0)
    {}
    
    AmbientLight::~AmbientLight(){}
    
    void AmbientLight::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
        shader->setUniform( "ambient_light_color", color, 1.0, gamma );
    }
}