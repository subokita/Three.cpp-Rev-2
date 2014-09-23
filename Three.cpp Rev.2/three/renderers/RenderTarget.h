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
        
    public:
        static ptr<RenderTarget> create( GLenum target, GLuint framebuffer = 0 );
        virtual ~RenderTarget();
        
        void bind();
        void unbind();
        void generateFrameBuffer();
    
    protected:
        RenderTarget( GLenum target, GLuint framebuffer );
        GLenum target;
        GLuint framebuffer;
        
/* FIXME: For now these ar ecommented out, they seems to be more of texture's properties */
//    public:
//        GLuint width;
//        GLuint height;
//        FILTER minFilter;
//        FILTER magFilter;
//        PIXEL_FORMAT format;

        
    };
}

#endif /* defined(__Three_cpp_Rev_2__RenderTarget__) */
