//
//  MeshPhongMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "MeshPhongMaterial.h"

using namespace std;

namespace three {
    ptr<MeshPhongMaterial> MeshPhongMaterial::create( Color color, Color ambient, Color emissive, Color specular,
                                                      float shininess, bool metal ) {
        return make_shared<MeshPhongMaterial>(MeshPhongMaterial( color, ambient, emissive, specular, shininess, metal ));
    }
    
    MeshPhongMaterial::MeshPhongMaterial():
        Material          (),
        color             ( Color(0xFFFFFF) ),
        ambient           ( Color(0xFFFFFF) ),
        emissive          ( Color(0x000000) ),
        specular          ( Color(0x111111) ),
        shininess         ( 30.0 ),
        metal             ( false ),
        wrapAround        ( false ),
        wrapRGB           ( glm::vec3(1.0, 1.0, 1.0) ),
//        bumpScale         ( 1.0 ),
//        normalScale       ( glm::vec2(1.0, 1.0) ),
        combine           ( MultiplyOperation ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        fog               ( true ),
        shading           ( SmoothShading ),
        vertexColors      ( NoColors ),
        skinning          ( false  ),
        morphTargets      ( false ),
        morphNormals      ( false )
    {}
    
    
    MeshPhongMaterial::MeshPhongMaterial( Color color, Color ambient, Color emissive, Color specular, float shininess, bool metal ):
        Material          (),
        color             (color),
        ambient           (ambient),
        emissive          (emissive),
        specular          (specular),
        shininess         (shininess),
        metal             (metal),
        wrapAround        ( false ),
        wrapRGB           ( glm::vec3(1.0, 1.0, 1.0) ),
//        bumpScale         ( 1.0 ),
//        normalScale       ( glm::vec2(1.0, 1.0) ),
        combine           ( MultiplyOperation ),
        reflectivity      ( 1.0 ),
        refractionRatio   ( 0.98 ),
        fog               ( true ),
        shading           ( SmoothShading ),
        vertexColors      ( NoColors ),
        skinning          ( false  ),
        morphTargets      ( false ),
        morphNormals      ( false )
    {
    }
    
    
    MeshPhongMaterial::~MeshPhongMaterial(){}
}