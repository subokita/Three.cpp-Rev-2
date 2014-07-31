//
//  PhongMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PhongMaterial.h"
#include "three.h"
#include "Shader.h"

using namespace std;

namespace three {
    ptr<PhongMaterial> PhongMaterial::create( Color color, Color ambient, Color emissive, Color specular,
                                                      float shininess, bool metal ) {
        return make_shared<PhongMaterial>(PhongMaterial( color, ambient, emissive, specular, shininess, metal ));
    }
    
    PhongMaterial::PhongMaterial():
        Material          (),
        color             ( 0xFFFFFF ),
        ambient           ( 0xFFFFFF ),
        emissive          ( 0x000000 ),
        specular          ( 0x111111 ),
        shininess         ( 30.0 ),
        metal             ( false ),
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
    
    
    PhongMaterial::PhongMaterial( Color color, Color ambient, Color emissive, Color specular, float shininess, bool metal ):
        Material          (),
        color             (color),
        ambient           (ambient),
        emissive          (emissive),
        specular          (specular),
        shininess         (shininess),
        metal             (metal),
        wrapAround        ( false ),
        wrapRGB           ( glm::vec3(1.0) ),
//        bumpScale         ( 1.0 ),
//        normalScale       ( glm::vec2(1.0, 1.0) ),
        combine           ( TEXTURE_OPERATION::MULTIPLY ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        useRefraction     ( false ),
        fog               ( true ),
        vertexColors      ( COLOR_MODE::NONE ),
        skinning          ( false  ),
        morphTargets      ( false ),
        morphNormals      ( false )
    {
    }
    
    
    PhongMaterial::~PhongMaterial(){}
    
    
    void PhongMaterial::setUniforms( ptr<Shader> shader, bool gamma ) {
        shader->setUniform( "opacity",   this->opacity );
        shader->setUniform( "emissive",  this->emissive, 1.0, gamma );
        shader->setUniform( "ambient",   this->ambient, 1.0, gamma );
        shader->setUniform( "diffuse",   this->color, 1.0, gamma );
        shader->setUniform( "specular",  this->specular, 1.0, gamma );
        shader->setUniform( "shininess", this->shininess );
        
        
        /*ENV MAP related*/
        shader->setUniform( "combine",          static_cast<int>(this->combine) );
        shader->setUniform( "reflectivity",     this->reflectivity );
        shader->setUniform( "refraction_ratio", this->refractionRatio );
        shader->setUniform( "flip_env_map",     (GLfloat) 1.0 );
        shader->setUniform( "use_refraction",   useRefraction );
        
    }
}