//
//  LambertMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 23/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "LambertMaterial.h"
#include "ShaderLib.h"
#include "Shader.h"


namespace three {
    ptr<LambertMaterial> LambertMaterial::create( Color diffuse, Color ambient, Color emissive ){
        return make_shared<LambertMaterial>(LambertMaterial(diffuse, ambient, emissive));
    }
    
    LambertMaterial::LambertMaterial():
        Material          (),
        color             ( 0xFFFFFF ),
        ambient           ( 0xFFFFFF ),
        emissive          ( 0x000000 ),
        wrapAround        ( false ),
        wrapRGB           ( glm::vec3(1.0, 1.0, 1.0) ),
        combine           ( TEXTURE_OPERATION::MULTIPLY ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        useRefraction     ( false ),
        fog               ( true ),
        vertexColors      ( COLOR_MODE::NONE ),
        skinning          ( false  ),
        morphTargets      ( false ),
        morphNormals      ( false )
    {}
    
    
    LambertMaterial::LambertMaterial( Color diffuse, Color ambient, Color emissive ):
        Material          (),
        color             ( diffuse ),
        ambient           ( ambient ),
        emissive          ( emissive ),
        wrapAround        ( false ),
        wrapRGB           ( glm::vec3(1.0, 1.0, 1.0) ),
        combine           ( TEXTURE_OPERATION::MULTIPLY ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        useRefraction     ( false ),
        fog               ( true ),
        vertexColors      ( COLOR_MODE::NONE ),
        skinning          ( false  ),
        morphTargets      ( false ),
        morphNormals      ( false )
    {}
    
    LambertMaterial::~LambertMaterial(){}
    
    void LambertMaterial::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
        shader->setUniform( "opacity",   this->opacity );
        shader->setUniform( "emissive",  this->emissive, 1.0, gamma );
        shader->setUniform( "ambient",   this->ambient, 1.0, gamma );
        shader->setUniform( "diffuse",   this->color, 1.0, gamma );
        
        /*ENV MAP related*/
        shader->setUniform( "combine",          static_cast<int>(this->combine) );
        shader->setUniform( "reflectivity",     this->reflectivity );
        shader->setUniform( "refraction_ratio", this->refractionRatio );
        shader->setUniform( "flip_env_map",     (GLfloat) 1.0 );
        shader->setUniform( "use_refraction",   useRefraction );
    }
}