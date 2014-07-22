//
//  SpecularMap.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 21/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "SpecularMap.h"
#include "three.h"

using namespace std;

namespace three {
    
    ptr<SpecularMap> SpecularMap::create() {
        return make_shared<SpecularMap>();
    }
    
    SpecularMap::SpecularMap() {}
    
    SpecularMap::~SpecularMap() {
        if( this->textureID != 0 )
            glDeleteTextures(1, &this->textureID );
    }
    
    
    void SpecularMap::setUniforms(ptr<Shader> shader, bool gamma) {
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, this->textureID );
        shader->setUniform( "specular_map" , 2 );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    }
}