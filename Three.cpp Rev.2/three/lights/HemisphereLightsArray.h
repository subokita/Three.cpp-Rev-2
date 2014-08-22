//
//  HemisphereLightsArray.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__HemisphereLightsArray__
#define __Three_cpp_Rev_2__HemisphereLightsArray__

#include <iostream>
#include <map>
#include "internal_headers.h"
#include "HemisphereLight.h"
#include "HasShaderUniforms.h"

namespace three {
    class HemisphereLightsArray: public HasShaderUniforms {
    public:
        HemisphereLightsArray();
        ~HemisphereLightsArray();
        
        const std::map<unsigned int, ptr<HemisphereLight>>& getCollection();
        unsigned int getSize();
        unsigned int getShadowCasterCount();
        
        void update();
        void add( ptr<Light> light );
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        
    protected:
        std::map<unsigned int, ptr<HemisphereLight>> lights;
        unsigned int size;
        std::vector<glm::vec3> directions;
        std::vector<Color> skyColors;
        std::vector<Color> groundColors;
        std::vector<float> intensities;
    };
}

#endif /* defined(__Three_cpp_Rev_2__HemisphereLightsArray__) */
