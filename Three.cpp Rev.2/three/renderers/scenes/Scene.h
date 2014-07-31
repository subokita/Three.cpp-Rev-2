//
//  Scene.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Scene__
#define __Three_cpp_Rev_2__Scene__

#include <iostream>
#include "Object3D.h"
#include "Mesh.h"
#include "AmbientLight.h"
#include "IFog.h"
#include "HasShaderUniforms.h"

#include "internal_headers.h"
#include "DirectionalLightsArray.h"
#include "HemisphereLightsArray.h"
#include "PointLightsArray.h"
#include "SpotLightsArray.h"

namespace three {
    class Scene : public Object3D, public HasShaderUniforms {
    public:
        static ptr<Scene> create();
        ~Scene();
        void add( ptr<Object3D> object );
        void add( ptr<Mesh> object );
        void addLight( ptr<Light> light );
        
        unsigned int getShadowCasterCount();
        void update();
        virtual void setUniforms( ptr<Shader> shader, bool gamma ) override;
        
    protected:
        Scene();
        
    public:
        bool autoUpdate;
        ptr<IFog> fog;
        ptr<AmbientLight> ambientLight;
        DirectionalLightsArray directionalLights;
        HemisphereLightsArray hemisphereLights;
        PointLightsArray pointLights;
        SpotLightsArray spotLights;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Scene__) */
