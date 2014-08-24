//
//  DirectionalLightsArray.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "DirectionalLightsArray.h"
#include "DirectionalLight.h"
#include "ShaderLib.h"
#include "Shader.h"
#include "MathUtils.h"

namespace three {
    
    DirectionalLightsArray::DirectionalLightsArray():
        size(0)
    {
    }
    
    DirectionalLightsArray::~DirectionalLightsArray(){
        lights.clear();
    }
    
    const std::map<unsigned int, ptr<DirectionalLight>>& DirectionalLightsArray::getCollection() {
        return lights;
    }
    
    unsigned int DirectionalLightsArray::getSize() {
        return size;
    }
    
    
    unsigned int DirectionalLightsArray::getShadowCasterCount() {
        unsigned int result = 0;
        for( auto entry: lights ){
            if( entry.second->visible && entry.second->castShadow )
                result++;
        }
        return result;
    }
    
    void DirectionalLightsArray::add( ptr<Light> light ){
        lights[light->id] = downcast(light, DirectionalLight);
    }
    
    void DirectionalLightsArray::update(){
        size = 0;
        directions.clear();
        colors.clear();
        intensities.clear();
        
        for( auto entry : lights ) {
            ptr<Light> light = entry.second;
            if( !light->visible )
                continue;
            
            size++;
            directions.push_back ( light->position );
            colors.push_back     ( light->getColor() );
            intensities.push_back( light->getIntensity() );
        }
    }
    
    void DirectionalLightsArray::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ){
        auto shader = shader_lib->getShader();
        
        shader->setUniform( "directional_light_direction", directions );
        shader->setUniform( "directional_light_color", colors, intensities, gamma );
    }
}