//
//  HemisphereLightsArray.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "HemisphereLightsArray.h"
#include "HemisphereLight.h"
#include "ShaderLib.h"
#include "Shader.h"
#include "MathUtils.h"
    
namespace three {
    
    HemisphereLightsArray::HemisphereLightsArray():
        size(0)
    {}
    
    HemisphereLightsArray::~HemisphereLightsArray(){
        lights.clear();
    }
    
    const std::map<unsigned int, ptr<HemisphereLight>>& HemisphereLightsArray::getCollection() {
        return lights;
    }
    
    unsigned int HemisphereLightsArray::getSize() {
        return size;
    }
    
    
    unsigned int HemisphereLightsArray::getShadowCasterCount() {
        unsigned int result = 0;
        for( auto entry: lights ){
            if( entry.second->visible && entry.second->castShadow )
                result++;
        }
        return result;
    }
    
    void HemisphereLightsArray::add( ptr<Light> light ){
        lights[light->id] = downcast(light, HemisphereLight);
    }
    
    void HemisphereLightsArray::update(){
        size = 0;
        directions.clear();
        skyColors.clear();
        groundColors.clear();
        intensities.clear();
        
        for( auto entry : lights ) {
            ptr<HemisphereLight> light = entry.second;
            if( !light->visible )
                continue;
            
            size++;
            skyColors.push_back    (light->getColor() );
            groundColors.push_back (light->getGroundColor());
            intensities.push_back  (light->getIntensity() );
            directions.push_back   (light->position);
        }
    }
    
    void HemisphereLightsArray::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ){
        auto shader = shader_lib->getShader();
        shader->setUniform( "hemisphere_light_sky_color", skyColors, intensities, gamma );
        shader->setUniform( "hemisphere_light_ground_color", groundColors, intensities, gamma );
        shader->setUniform( "hemisphere_light_direction", directions );
    }
}