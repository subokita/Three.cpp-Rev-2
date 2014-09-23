//
//  DeferredRenderer.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__DeferredRenderer__
#define __Three_cpp_Rev_2__DeferredRenderer__

#include <iostream>
#include "Renderer.h"

namespace three {
    class DeferredRenderer: public Renderer {
        public:
            virtual void init( std::string window_title, GLuint window_width, GLuint window_height ) override;
            virtual void render( ptr<Scene> scene, ptr<Camera> camera ) override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__DeferredRenderer__) */
