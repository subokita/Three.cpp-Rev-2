//
//  ShadowTexture.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 29/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__ShadowTexture__
#define __Three_cpp_Rev_2__ShadowTexture__

#include <iostream>
#include "internal_headers.h"
#include "Texture.h"

namespace three {
    class ShadowTexture : public Texture {
    public:
        static ptr<ShadowTexture> create(GLuint wrap_s, GLuint wrap_t, GLuint wrap_r,
                                         GLuint mag_filter, GLuint min_filter);
        
        ~ShadowTexture();
        virtual void setUniforms(ptr<ShaderLib> shader_lib, bool gamma);
        
    protected:
        ShadowTexture(GLuint wrap_s, GLuint wrap_t, GLuint wrap_r,
                      GLuint mag_filter, GLuint min_filter);
        
    };
}


#endif /* defined(__Three_cpp_Rev_2__ShadowTexture__) */
