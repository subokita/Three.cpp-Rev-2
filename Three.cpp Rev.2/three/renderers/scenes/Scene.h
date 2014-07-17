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
#include "Light.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "HemisphereLight.h"
#include "SpotLight.h"
#include "IFog.h"

namespace three {
    class Scene : public Object3D {
    public:
        static PTR(Scene) create();
        ~Scene();
        Scene& add( PTR(Object3D) object );
        Scene& add( PTR(Mesh) object );
        Scene& addLight( PTR(Light) light );
        
        PTR(IFog) fog;
        PTR(AmbientLight) ambientLight;
        std::vector<PTR(DirectionalLight)> directionalLights;
        std::vector<PTR(HemisphereLight)> hemisphereLights;
        std::vector<PTR(PointLight)> pointLights;
        std::vector<PTR(SpotLight)> spotLights;
        
    protected:
        Scene();
    };
}

#endif /* defined(__Three_cpp_Rev_2__Scene__) */
