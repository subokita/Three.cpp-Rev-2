//
//  SpotLightsArray.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "SpotLightsArray.h"
#include "SpotLight.h"
#include "ShaderLib.h"
#include "Shader.h"
#include "MathUtils.h"

namespace three {
    
    SpotLightsArray::SpotLightsArray():
        size(0)
    {}
    
    SpotLightsArray::~SpotLightsArray(){
        lights.clear();
    }
    
    const std::map<unsigned int, ptr<SpotLight>>& SpotLightsArray::getCollection() {
        return lights;
    }
    
    unsigned int SpotLightsArray::getSize() {
        return size;
    }
    
    
    unsigned int SpotLightsArray::getShadowCasterCount() {
        unsigned int result = 0;
        for( auto entry: lights ){
            if( entry.second->visible && entry.second->castShadow )
                result++;
        }
        return result;
    }
    
    void SpotLightsArray::add( ptr<Light> light ){
        lights[light->id] = downcast(light, SpotLight);
    }
    
    void SpotLightsArray::update(){
        size = 0;
        colors.clear();
        intensities.clear();
        positions.clear();
        directions.clear();
        cos_angles.clear();
        exponents.clear();
        distances.clear();

        for( auto entry : lights ) {
            ptr<SpotLight> light = entry.second;
            if( !light->visible )
                continue;
            
            size++;
            colors.push_back     (light->getColor() );
            intensities.push_back(light->getIntensity() );
            positions.push_back  (light->position);
            directions.push_back (glm::normalize(light->position - light->target->position));
            cos_angles.push_back (cosf(MathUtils::degToRad(light->angle)));
            exponents.push_back  (light->exponent);
            distances.push_back  (light->distance);
        }
    }
    
    void SpotLightsArray::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ){
        auto shader = shader_lib->getShader();
        shader->setUniform( "spot_light_color", colors, intensities, gamma );
        shader->setUniform( "spot_light_position", positions );
        shader->setUniform( "spot_light_direction", directions );
        
        shader->setUniform( "spot_light_angle_cos", cos_angles );
        shader->setUniform( "spot_light_exponent", exponents );
        shader->setUniform( "spot_light_distance", distances );
    }
}