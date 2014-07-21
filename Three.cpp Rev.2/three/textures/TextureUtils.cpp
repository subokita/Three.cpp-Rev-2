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
    
    ptr<fipImage> TextureUtils::loadImage( const string path, const string filename ) {
        // cout << path + "/" + filename << endl;
        
        ptr<fipImage> image = make_shared<fipImage>();
        
        if( image->load( (path + "/" + filename).c_str() ) == false ) {
            image->clear();
            string error_message = "Unable to load image " + path + "/" + filename;
            throw runtime_error( error_message );
        }
        return image;
    }
    
    
    ptr<EnvMap> TextureUtils::loadAsEnvMap( const string path,
                                            const string neg_x, const string neg_y, const string neg_z,
                                            const string pos_x, const string pos_y, const string pos_z ) {
        
        ptr<EnvMap> map = make_shared<EnvMap>();
        
        glGenTextures(1, &map->textureID);
        glBindTexture( GL_TEXTURE_CUBE_MAP, map->textureID );
        
        const vector<std::string> filenames = {
            neg_x, neg_y, neg_z, pos_x, pos_y, pos_z
        };
        
        const static vector<GLenum> targets = {
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        };
        
        for ( int i = 0; i < 6; i++ ) {
            ptr<fipImage> image = loadImage(path, filenames[i]);
            
            map->width  = image->getWidth();
            map->height = image->getHeight();

            image->convertTo32Bits();
            glTexImage2D( targets[i], 0, GL_RGBA, map->width, map->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
            
            image = nullptr;
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        return map;
    }
    
    ptr<SpecularMap> TextureUtils::loadAsSpecularMap( const string path, const string filename ) {
        ptr<fipImage> image = loadImage(path, filename);
        ptr<SpecularMap> map = make_shared<SpecularMap>();
        
        map->width  = image->getWidth();
        map->height = image->getHeight();
        
        image->convertToGrayscale();
        
        size_t size = map->width * map->height;
        
        vector<float> speculars(size);
        BYTE* data = image->accessPixels();
        
        for( int i = 0; i < size; i++ )
            speculars[i] = static_cast<float>(data[i]) / 255.0;
        
        glGenTextures(1, &map->textureID);
        glBindTexture( GL_TEXTURE_2D, map->textureID );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, map->width, map->height, 0, GL_RED, GL_FLOAT, &speculars[0]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        image->clear();
        image = nullptr;
        
        return map;
    }
    
    ptr<NormalMap> TextureUtils::loadAsNormalMap( const string path, const string filename ) {
        ptr<fipImage> image = loadImage(path, filename);
        ptr<NormalMap> map = make_shared<NormalMap>();
        
        map->width  = image->getWidth();
        map->height = image->getHeight();
        
        size_t size = map->width * map->height;
        
        image->convertTo24Bits();
        
        vector<glm::vec3> normals(size, glm::vec3(0.0));
        BYTE* data = image->accessPixels();
        
        for( int i = 0, j = 0; i < size * 3; i+=3, j++ ) {
            normals[j].x = static_cast<int>(data[i  ]) / 255.0;
            normals[j].y = static_cast<int>(data[i+1]) / 255.0;
            normals[j].z = static_cast<int>(data[i+2]) / 255.0;
        }
        
        glGenTextures(1, &map->textureID);
        glBindTexture( GL_TEXTURE_2D, map->textureID );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, map->width, map->height, 0, GL_BGR, GL_FLOAT, &normals[0]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        image->clear();
        image = nullptr;
        
        return map;
    }
    
    ptr<Texture> TextureUtils::loadImageAsTexture( const string path, const string filename ) {
        ptr<fipImage> image = loadImage(path, filename);
        
        ptr<Texture> texture = make_shared<Texture>();
        texture->width  = image->getWidth();
        texture->height = image->getHeight();
        
        glGenTextures(1, &texture->textureID);
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        
        switch( image->getColorType() ) {
            case FIC_RGB:
                image->convertTo32Bits();
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
                
                break;
            case FIC_PALETTE:
                image->convertTo32Bits();
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
                break;
            default:
                throw runtime_error( "Unhandled image type" );
                break;
        }
        
        image->clear();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return texture;
    }
    
    
    ptr<Texture> TextureUtils::loadEmptyWhiteTexture() {
        ptr<Texture> texture = make_shared<Texture>();
        texture->width  = 1;
        texture->height = 1;

        glGenTextures(1, &texture->textureID);
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        
        vector<GLubyte> empty_data(3, 255);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, &empty_data[0]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return texture;
    }

}