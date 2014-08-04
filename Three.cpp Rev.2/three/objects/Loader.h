//
//  Loader.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Loader__
#define __Three_cpp_Rev_2__Loader__

#include <iostream>
#include "internal_headers.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace three {
    class Loader {
    public:
        static ptr<Mesh> loadPLY( const std::string path, const std::string filename );
        
    protected:
        static inline glm::vec2 toGLMVec2(aiVector3D& ai_vector);
        static inline glm::vec3 toGLMVec3(aiVector3D& ai_vector);
    };
}

#endif /* defined(__Three_cpp_Rev_2__Loader__) */
