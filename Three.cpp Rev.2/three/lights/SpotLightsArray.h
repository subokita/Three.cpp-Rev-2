//
//  SpotLightsArray.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__SpotLightsArray__
#define __Three_cpp_Rev_2__SpotLightsArray__
#include <iostream>
#include <map>
#include "internal_headers.h"
#include "SpotLight.h"
#include "HasShaderUniforms.h"

namespace three {
    class SpotLightsArray: public HasShaderUniforms {
    public:
        SpotLightsArray();
        ~SpotLightsArray();
        
        const std::map<unsigned int, ptr<SpotLight>>& getCollection();
        unsigned int getSize();
        unsigned int getShadowCasterCount();
        
        void update();
        void add( ptr<Light> light );
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        
    protected:
        std::map<unsigned int, ptr<SpotLight>> lights;
        unsigned int size;

        std::vector<Color> colors;
        std::vector<float> intensities;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> directions;
        std::vector<float> cos_angles;
        std::vector<float> exponents;
        std::vector<float> distances;
    };
}


#endif /* defined(__Three_cpp_Rev_2__SpotLightsArray__) */
