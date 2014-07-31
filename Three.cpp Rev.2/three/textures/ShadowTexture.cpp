//
//  ShadowTexture.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 29/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ShadowTexture.h"
#include "three.h"

using namespace std;

namespace three  {
    ptr<ShadowTexture> ShadowTexture::create(GLuint wrap_s, GLuint wrap_t, GLuint wrap_r, GLuint mag_filter, GLuint min_filter){
        return make_shared<ShadowTexture>( ShadowTexture(wrap_s, wrap_t, wrap_r, mag_filter, min_filter) );
    }
    
    ShadowTexture::ShadowTexture(GLuint wrap_s, GLuint wrap_t, GLuint wrap_r, GLuint mag_filter, GLuint min_filter):
        Texture(TEXTURE_TYPE::TEXTURE_2D, wrap_s, wrap_t, wrap_r, mag_filter, min_filter)
    {}
    
    ShadowTexture::~ShadowTexture(){
        if( this->textureID != 0 )
            glDeleteTextures(1, &this->textureID );
    }
    
    void ShadowTexture::setUniforms(ptr<Shader> shader, bool gamma) {
        glActiveTexture( GL_TEXTURE4 );
        bind();
        shader->setUniform( "shadowMap", 0 );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
        
        
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
    }
}