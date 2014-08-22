//
//  DirectionalLightsArray.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__DirectionalLightsArray__
#define __Three_cpp_Rev_2__DirectionalLightsArray__

#include <iostream>
#include <map>
#include "internal_headers.h"
#include "DirectionalLight.h"
#include "HasShaderUniforms.h"

namespace three {
    class DirectionalLightsArray: public HasShaderUniforms {
    public:
        DirectionalLightsArray();
        ~DirectionalLightsArray();
        
        const std::map<unsigned int, ptr<DirectionalLight>>& getCollection();
        unsigned int getSize();
        unsigned int getShadowCasterCount();
        
        void update();
        void add( ptr<Light> light );
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        
    protected:
        std::map<unsigned int, ptr<DirectionalLight>> lights;
        unsigned int size;
        std::vector<glm::vec3> directions;
        std::vector<Color> colors;
        std::vector<float> intensities;
    };
}

#endif /* defined(__Three_cpp_Rev_2__DirectionalLightsArray__) */
