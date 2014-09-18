//
//  ParticleSystemMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__ParticleSystemMaterial__
#define __Three_cpp_Rev_2__ParticleSystemMaterial__

#include <iostream>

#include "internal_headers.h"
#include "Material.h"
#include "Color.h"

namespace three {
    class ParticleSystemMaterial : public Material {
    public:
        static ptr<ParticleSystemMaterial> create();
        ParticleSystemMaterial();
        ~ParticleSystemMaterial();
        
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        void setColor( Color color );
        void setSizeAttenuation( bool flag );
        
    protected:
        Color color;
        bool sizeAttenuation;
    };
}

#endif /* defined(__Three_cpp_Rev_2__ParticleSystemMaterial__) */
