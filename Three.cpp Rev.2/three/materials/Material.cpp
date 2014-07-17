//
//  Material.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Material.h"
#include "Constants.h"

namespace three {
    Material::Material() :
        HasID(MaterialIDCount++)
    {
        // FIXME: should be in initializer list
        
        side                = three::FrontSide;
        opacity             = 1.0;
        transparent         = false;
        
        blending            = three::NormalBlending;
        blendSrc            = three::SrcAlphaFactor;
        blendDst            = three::OneMinusSrcAlphaFactor;
        blendEquation       = three::AddEquation;
        
        depthTest           = true;
        depthWrite          = true;
        
        polygonOffset       = false;
        polygonOffsetFactor = 0.0;
        polygonOffsetUnits  = 0.0;
        
        alphaTest           = 0.0;
        overdraw            = 0.0;
        
        visible             = true;
        needsUpdate         = true;
        
        wireframe           = false;
        wireframeLinewidth  = 1;
        wireframeLinecap    = "round";
        wireframeLinejoin   = "round";
    }
    
    Material::~Material() {
        
    }
}