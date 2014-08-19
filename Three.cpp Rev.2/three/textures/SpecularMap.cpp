//
//  SpecularMap.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 21/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "SpecularMap.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<SpecularMap> SpecularMap::create() {
        return make_shared<SpecularMap>();
    }
    
    SpecularMap::SpecularMap():
        Texture(TEXTURE_TYPE::TEXTURE_2D)
    {}
    
    SpecularMap::~SpecularMap() {
        if( this->textureID != 0 )
            glDeleteTextures(1, &this->textureID );
    }
    
    
    void SpecularMap::setUniforms(ptr<ShaderLib> shader_lib, bool gamma) {
        auto shader = shader_lib->getShader();
        
        int offset = shader_lib->getConfig()[0] + shader_lib->getConfig()[1];
        
        glActiveTexture( GL_TEXTURE0 + offset );
        bind();
        shader->setUniform( "specular_map" , offset );
        
        glm::vec4 offset_repeat( this->offset.x, this->offset.y, this->repeat.x, this->repeat.y );
        shader->setUniform( "offset_repeat", offset_repeat );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    }
}