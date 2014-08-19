//
//  PhongMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PhongMaterial.h"
#include "ShaderLib.h"
#include "Shader.h"


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
        combine           ( TEXTURE_OPERATION::MULTIPLY ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        useRefraction     ( false ),
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
//        bumpScale         ( 1.0 ),
//        normalScale       ( glm::vec2(1.0, 1.0) ),
        combine           ( TEXTURE_OPERATION::MULTIPLY ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        useRefraction     ( false ),
        vertexColors      ( COLOR_MODE::NONE ),
        skinning          ( false  ),
        morphTargets      ( false ),
        morphNormals      ( false )
    {
    }
    
    
    PhongMaterial::~PhongMaterial(){}
    
    
    void PhongMaterial::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
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
    
    
    const Color& PhongMaterial::getDiffuseColor() {
        return this->color;
    }
    
    
    void PhongMaterial::setDiffuseColor( const Color color ) {
        this->color = color;
    }
    
    const Color& PhongMaterial::getEmissiveColor() {
        return this->emissive;
    }
    
    void PhongMaterial::setEmissiveColor( const Color color ) {
        this->emissive = color;
    }
    
    const Color& PhongMaterial::getAmbientColor() {
        return this->ambient;
    }
    
    void PhongMaterial::setAmbientColor( const Color color ) {
        this->ambient = color;
    }
    
    const Color& PhongMaterial::getSpecularColor() {
        return this->specular;
    }
    
    void PhongMaterial::setSpecularColor( const Color color ) {
        this->specular = color;
    }
    
    float PhongMaterial::getShininess() {
        return shininess;
    }
    bool PhongMaterial::isMetal() {
        return metal;
    }
    bool PhongMaterial::usesRefraction() {
        return useRefraction;
    }
    
    void PhongMaterial::setShininess( float shininess ) {
        this->shininess = shininess;
    }
    void PhongMaterial::setMetal( bool flag ) {
        this->metal = flag;
    }
    void PhongMaterial::setRefraction( bool flag ) {
        this->useRefraction = flag;
    }
    
    
    const TEXTURE_OPERATION PhongMaterial::getTextureOperation() {
        return combine;
    }
    
    void PhongMaterial::setTextureOperation( TEXTURE_OPERATION op ) {
        this->combine = op;
    }
    float PhongMaterial::getReflectivity() {
        return reflectivity;
    }
    void PhongMaterial::setReflectivity( float value ) {
        this->reflectivity = value;
    }
    
    float PhongMaterial::getRefractionRatio() {
        return refractionRatio;
    }
    
    void PhongMaterial::setRefractionRatio( float value ) {
        this->refractionRatio = value;
    }
    
    const COLOR_MODE PhongMaterial::getVertexColorsMode() {
        return vertexColors;
    }
    
    void PhongMaterial::setVertexColorsMode( COLOR_MODE mode ) {
        this->vertexColors = mode;
    }
}