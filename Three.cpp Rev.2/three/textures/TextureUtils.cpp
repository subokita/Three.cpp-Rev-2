//
//  TextureUtils.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "TextureUtils.h"
#include "three.h"

using namespace std;

namespace three {
    
    FIBITMAP * TextureUtils::loadImage( const std::string path, const std::string filename ) {
        const char * file = (path + filename).c_str();
        
        FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(file), file);
        if( bitmap == NULL )
            throw runtime_error( "Unable to load file" );
        
        return bitmap;
    }
    
    PTR(Texture) TextureUtils::loadImageAsTexture( const std::string path, const std::string filename ) {
        FIBITMAP* bitmap = loadImage( path, filename );
        PTR(Texture) texture = make_shared<Texture>();
        
        glGenTextures(1, &texture->textureID);
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        
        if( FreeImage_GetColorType( bitmap ) == FIC_RGB ) {
            RGBQUAD * quad;
            FreeImage_GetPixelColor( bitmap, 1, 1, quad );
            
            bitmap = FreeImage_ConvertTo24Bits( bitmap );
            if( quad->rgbBlue != 0 ) {
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), 0, GL_BGR, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits( bitmap ));
            }
            else {
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), 0, GL_BGR, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits( bitmap ));
            }
        }
        else {
            
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits( bitmap ));
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        FreeImage_Unload( bitmap );
        
        return texture;
    }
    
    
    PTR(Texture) TextureUtils::loadEmptyWhiteTexture() {
        PTR(Texture) texture = make_shared<Texture>();

        glGenTextures(1, &texture->textureID);
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        
        vector<GLubyte> empty_data(3, 255);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, &empty_data[0]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return texture;
    }

}