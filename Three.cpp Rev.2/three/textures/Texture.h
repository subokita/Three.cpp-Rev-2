//
//  Texture.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Texture__
#define __Three_cpp_Rev_2__Texture__

#include <iostream>

#include "internal_headers.h"
#include "EventDispatcher.h"
#include "HasID.h"
#include "HasShaderUniforms.h"

namespace three {
    static unsigned int textureIDCount = 0;
    
    class Texture : public HasID, public HasShaderUniforms, public EventDispatcher {
    public:
        static ptr<Texture> create(TEXTURE_TYPE type = TEXTURE_TYPE::TEXTURE_2D);
        Texture(TEXTURE_TYPE type);
        Texture(TEXTURE_TYPE type, GLuint wrap_s, GLuint wrap_t, GLuint wrap_r, GLuint mag_filter, GLuint min_filter);
        ~Texture();
        
        const GLuint genTexture();
        
        void bind();
        virtual void setUniforms(ptr<ShaderLib> shader_lib, bool gamma) override;
        void setWrap();
        
        
    public:
        GLuint textureID;
        uint width;
        uint height;
        
        TEXTURE_TYPE type;

        GLuint wrapS;
        GLuint wrapT;
        GLuint wrapR;
        GLuint magFilter;
        GLuint minFilter;
        int anisotropy;
        GLuint format;
        
        glm::vec2 offset;
        glm::vec2 repeat;
        
    protected:
        unsigned int unpackAlignment;
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__Texture__) */
