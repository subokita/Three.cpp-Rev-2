//
//  TextureUtils.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__TextureUtils__
#define __Three_cpp_Rev_2__TextureUtils__

#include <iostream>
#include "internal_headers.h"
#include <FreeImagePlus.h>

namespace three {
    class TextureUtils{
    public:
        static FIBITMAP* loadImage( const std::string path, const std::string filename );
        static ptr<Texture> loadImageAsTexture( const std::string path, const std::string filename );
        static ptr<Texture> loadEmptyWhiteTexture();
        static ptr<NormalTexture> loadAsNormalMap( const std::string path, const std::string filename );
    };
}

#endif /* defined(__Three_cpp_Rev_2__TextureUtils__) */
