//
//  PointLightsArray.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PointLightsArray.h"
#include "PointLight.h"
#include "ShaderLib.h"
#include "Shader.h"


using namespace std;
namespace three {
    
    PointLightsArray::PointLightsArray():
        size(0)
    {}
    
    PointLightsArray::~PointLightsArray(){
        lights.clear();
    }
    
    const std::map<unsigned int, ptr<PointLight>>& PointLightsArray::getCollection() {
        return lights;
    }
    
    unsigned int PointLightsArray::getSize() {
        return size;
    }
    
    
    unsigned int PointLightsArray::getShadowCasterCount() {
        unsigned int result = 0;
        for( auto entry: lights ){
            if( entry.second->visible && entry.second->castShadow )
                result++;
        }
        return result;
    }
    
    void PointLightsArray::add( ptr<Light> light ){
        lights[light->id] = downcast(light, PointLight);
    }
    
    void PointLightsArray::update(){
        size = 0;
        colors.clear();
        intensities.clear();
        positions.clear();
        distances.clear();
        
        for( auto entry : lights ) {
            ptr<PointLight> light = entry.second;
            if( !light->visible )
                continue;
            
            size++;
            colors.push_back     (light->getColor() );
            intensities.push_back(light->getIntensity() );
            positions.push_back  (light->position);
            distances.push_back  (light->getDistance());
        }
    }
    
    void PointLightsArray::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ){
        auto shader = shader_lib->getShader();
        shader->setUniform( "point_light_color", colors, intensities, gamma );
        shader->setUniform( "point_light_position", positions );
        shader->setUniform( "point_light_distance", distances );
    }
}