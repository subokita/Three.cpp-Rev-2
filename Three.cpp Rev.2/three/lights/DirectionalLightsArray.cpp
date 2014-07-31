//
//  DirectionalLightsArray.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "DirectionalLightsArray.h"
#include "three.h"

using namespace std;
namespace three {
    
    DirectionalLightsArray::DirectionalLightsArray():
        size(0)
    {}
    
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
            colors.push_back     ( light->color );
            intensities.push_back( light->intensity );
        }
    }
    
    void DirectionalLightsArray::setUniforms( ptr<Shader> shader, bool gamma ){
        for( auto entry : lights ) {
            ptr<DirectionalLight> light = entry.second;
            if( !light->visible )
                continue;

            /*FIXME: refactor out to setShadows*/
            if( light->shadowMap != nullptr ) {
                light->shadowTexture->setUniforms( shader, gamma );
                shader->setUniform( "shadow_mat", light->shadowMatrix );
                shader->setUniform( "shadow_bias", light->shadowBias);
                shader->setUniform( "shadow_darkness", light->shadowDarkness );
                shader->setUniform( "shadow_map_size", light->shadowMapSize );
            }
        }

        shader->setUniform( "directional_light_direction", directions );
        shader->setUniform( "directional_light_color", colors, intensities, gamma );
    }
}