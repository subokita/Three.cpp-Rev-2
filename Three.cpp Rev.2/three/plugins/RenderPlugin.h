//
//  RenderPlugin.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__RenderPlugin__
#define __Three_cpp_Rev_2__RenderPlugin__

#include <iostream>
#include "internal_headers.h"

namespace three {
    class RenderPlugin {
    public:
        RenderPlugin();
        ~RenderPlugin();
        virtual void setState(ptr<Scene>, ptr<Camera>) = 0;
        virtual void init( ptr<Scene>, ptr<Camera> ) = 0;
        virtual void render( ptr<Scene>, ptr<Camera> ) = 0;
        
    protected:
        std::vector<ptr<Object3D>> descendants;
    };
}

#endif /* defined(__Three_cpp_Rev_2__RenderPlugin__) */
