//
//  ParticleSystemMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ParticleSystemMaterial.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<ParticleSystemMaterial> ParticleSystemMaterial::create() {
        return make_shared<ParticleSystemMaterial>();
    }
    
    ParticleSystemMaterial::ParticleSystemMaterial() {
        this->setPolygonMode(POLYGON_MODE::POINT);
    }
    
    ParticleSystemMaterial::~ParticleSystemMaterial() {
        
    }
    
    void ParticleSystemMaterial::setColor( Color color ) {
        this->color = color;
    }
    
    void ParticleSystemMaterial::setSizeAttenuation( bool flag ) {
        this->sizeAttenuation = flag;
    }
    
    void ParticleSystemMaterial::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();

        shader->setUniform( "opacity",   this->opacity );
        shader->setUniform( "particle_color",   this->color, 1.0, gamma );
        
    }
}