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
        static ptr<Texture> create();
        Texture();
        Texture(GLuint wrap_s, GLuint wrap_t, GLuint wrap_r, GLuint mag_filter, GLuint min_filter);
        ~Texture();
        
        virtual void setUniforms(ptr<Shader> shader, bool gamma) override;
        
        unsigned int width;
        unsigned int height;
        
        GLuint textureID;
        GLuint wrapS;
        GLuint wrapT;
        GLuint wrapR;
        GLuint magFilter;
        GLuint minFilter;
        int anisotropy;
        GLuint format;
        GLuint type;
        
        glm::vec2 offset;
        glm::vec2 repeat;
        unsigned int unpackAlignment;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Texture__) */
