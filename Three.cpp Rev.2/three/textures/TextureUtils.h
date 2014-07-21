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
        static ptr<fipImage> loadImage( const std::string path, const std::string filename );
        static ptr<Texture> loadImageAsTexture( const std::string path, const std::string filename );
        static ptr<Texture> loadEmptyWhiteTexture();
        static ptr<SpecularMap> loadAsSpecularMap( const std::string path, const std::string filename );
        static ptr<NormalMap> loadAsNormalMap( const std::string path, const std::string filename );
        static ptr<EnvMap> loadAsEnvMap( const std::string path,
                                         const std::string neg_x, const std::string neg_y, const std::string neg_z,
                                         const std::string pos_x, const std::string pos_y, const std::string pos_z );
    };
}

#endif /* defined(__Three_cpp_Rev_2__TextureUtils__) */
