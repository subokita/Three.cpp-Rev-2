//
//  EnvMap.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 21/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "EnvMap.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    ptr<EnvMap> EnvMap::create() {
        return make_shared<EnvMap>();
    }
    
    EnvMap::EnvMap():
        Texture( TEXTURE_TYPE::TEXTURE_CUBEMAP, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR )
    {}
    
    EnvMap::~EnvMap(){
        if( this->textureID != 0 )
            glDeleteTextures(1, &this->textureID );
    }
    
    
    void EnvMap::setUniforms(ptr<ShaderLib> shader_lib, bool gamma) {
        auto shader = shader_lib->getShader();
        
        //FIXME: setting it to lower active texture no seems to interfere with other texture
        // Maybe TEXTURE_CUBE_MAP actually requires 6 slots??
        int offset  = shader_lib->getConfig()[0]
                    + shader_lib->getConfig()[1]
                    + shader_lib->getConfig()[2];
        
        glActiveTexture( GL_TEXTURE0 + offset );
        glBindTexture( GL_TEXTURE_CUBE_MAP, this->textureID );
        shader->setUniform( "env_map", offset );
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR );
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS );
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT );
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    }
}