//
//  ShadowTexture.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 29/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ShadowTexture.h"
#include "ShaderLib.h"
#include "Shader.h"


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
    
    void ShadowTexture::setUniforms(ptr<ShaderLib> shader_lib, bool gamma) {
        auto shader = shader_lib->getShader();
        
        int offset = shader_lib->getConfig()[0] + shader_lib->getConfig()[1] + shader_lib->getConfig()[2] + shader_lib->getConfig()[3];
        
        glActiveTexture( GL_TEXTURE0 + offset );
        bind();
        shader->setUniform( "shadowMap", offset  );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}