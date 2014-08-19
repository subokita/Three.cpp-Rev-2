//
//  LineBasicMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 12/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "LineBasicMaterial.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<LineBasicMaterial> LineBasicMaterial::create( Color color ) {
        return make_shared<LineBasicMaterial>(LineBasicMaterial(color));
    }
    
    LineBasicMaterial::~LineBasicMaterial() {
        
    }
    
    LineBasicMaterial::LineBasicMaterial() {
        Material::polygonMode = POLYGON_MODE::WIREFRAME;
    }
    
    LineBasicMaterial::LineBasicMaterial(Color color) :
        BasicMaterial(color)
    {
        Material::polygonMode = POLYGON_MODE::WIREFRAME;
    }
}