//
//  PointLightsArray.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PointLightsArray__
#define __Three_cpp_Rev_2__PointLightsArray__
#include <iostream>
#include <map>
#include "internal_headers.h"
#include "PointLight.h"
#include "HasShaderUniforms.h"

namespace three {
    class PointLightsArray: public HasShaderUniforms {
    public:
        PointLightsArray();
        ~PointLightsArray();
        
        const std::map<unsigned int, ptr<PointLight>>& getCollection();
        unsigned int getSize();
        unsigned int getShadowCasterCount();
        
        void update();
        void add( ptr<Light> light );
        virtual void setUniforms( ptr<Shader> shader, bool gamma ) override;
        
        
    protected:
        std::map<unsigned int, ptr<PointLight>> lights;
        unsigned int size;
        std::vector<glm::vec3> positions;
        std::vector<Color> colors;
        std::vector<float> distances;
        std::vector<float> intensities;
    };
}

#endif /* defined(__Three_cpp_Rev_2__PointLightsArray__) */
