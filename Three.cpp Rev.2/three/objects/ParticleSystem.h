//
//  ParticleSystem.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__ParticleSystem__
#define __Three_cpp_Rev_2__ParticleSystem__

#include <iostream>

#include "internal_headers.h"
#include "Mesh.h"
#include "Constants.h"
#include "Camera.h"

namespace three {
    class ParticleSystem : public Mesh {
    public:
        static ptr<ParticleSystem> create(ptr<Geometry> geometry, ptr<Material> material);
        
        ParticleSystem();
        virtual ~ParticleSystem();
        
        void sort(ptr<Camera> camera);
        void setSortParticles( bool flag );
        
        virtual void initGLBuffers() override;
        virtual void setUniforms(ptr<ShaderLib> shader_lib, bool gamma ) override;
        virtual void draw() override;
        virtual void setGLState() override;

    protected:
        bool sortParticles;
    };
}

#endif /* defined(__Three_cpp_Rev_2__ParticleSystem__) */
