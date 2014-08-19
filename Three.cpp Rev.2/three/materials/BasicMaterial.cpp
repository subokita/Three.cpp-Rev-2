//
//  BasicMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 23/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "BasicMaterial.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<BasicMaterial> BasicMaterial::create( Color color){
        return make_shared<BasicMaterial>(BasicMaterial(color));
    }
    
    BasicMaterial::BasicMaterial(Color color) :
        color( color )
    {}
    
    BasicMaterial::BasicMaterial(){}
    
    BasicMaterial::~BasicMaterial(){}
    
    void BasicMaterial::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ){
        auto shader = shader_lib->getShader();
        shader->setUniform( "opacity",   this->opacity );
        shader->setUniform( "diffuse",   this->color, 1.0, gamma );
    }
}