//
//  RenderTarget.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__RenderTarget__
#define __Three_cpp_Rev_2__RenderTarget__

#include <iostream>
#include "internal_headers.h"

namespace three {
    class RenderTarget {
        friend class Renderer;
        
    public:
        static ptr<RenderTarget> create( GLenum target, GLuint framebuffer );
        RenderTarget( GLenum target, GLuint framebuffer );
        ~RenderTarget();
        
        void bind();
        
    protected:
        GLenum target;
        GLuint framebuffer;
    };
}

#endif /* defined(__Three_cpp_Rev_2__RenderTarget__) */
