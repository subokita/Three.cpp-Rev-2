//
//  Texture.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Texture.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<Texture> Texture::create(TEXTURE_TYPE type) {
        return make_shared<Texture>(Texture(type));
    }

    Texture::Texture(TEXTURE_TYPE type) :
        HasID    ( textureIDCount++ ),
        type     ( type ),
        textureID( 0 ),
        width    ( 0 ),
        height   ( 0 ),
        wrapS    ( GL_REPEAT ),
        wrapT    ( GL_REPEAT ),
        wrapR    ( GL_REPEAT ),
        magFilter( GL_LINEAR ),
        minFilter( GL_LINEAR ),
        offset   ( glm::vec2(0.0, 0.0) ),
        repeat   ( glm::vec2(1.0, 1.0) )
    {}
    
    
    Texture::Texture(TEXTURE_TYPE type, GLuint wrap_s, GLuint wrap_t, GLuint wrap_r, GLuint mag_filter, GLuint min_filter) :
        HasID    ( textureIDCount++ ),
        type     ( type ),
        textureID( 0 ),
        width    ( 0 ),
        height   ( 0 ),
        wrapS    ( wrap_s ),
        wrapT    ( wrap_t ),
        wrapR    ( wrap_r ),
        magFilter( mag_filter ),
        minFilter( min_filter ),
        offset   ( glm::vec2(0.0, 0.0) ),
        repeat   ( glm::vec2(1.0, 1.0) )
    {}
    
    Texture::~Texture(){
        if( this->textureID != 0 )
            glDeleteTextures(1, &this->textureID );
    }
    
    
    const GLuint Texture::genTexture() {
        glGenTextures(1, &this->textureID);
        return this->textureID;
    }
    
    
    void Texture::bind() {
        glBindTexture(static_cast<GLuint>(this->type) , this->textureID);
    }
    
    void Texture::setUniforms(ptr<ShaderLib> shader_lib, bool gamma) {
        auto shader = shader_lib->getShader();
        
        glActiveTexture( GL_TEXTURE0 );
        bind();
        shader->setUniform("map", 0 );

        glm::vec4 offset_repeat( this->offset.x, this->offset.y, this->repeat.x, this->repeat.y );
        shader->setUniform( "offset_repeat", offset_repeat );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    }
}