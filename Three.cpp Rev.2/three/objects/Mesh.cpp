//
//  Mesh.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Mesh.h"
#include "three.h"

using namespace std;

namespace three {
    ptr<Mesh> Mesh::create(){
        return make_shared<Mesh>();
    }
    
    ptr<Mesh> Mesh::create(ptr<Geometry> geometry, ptr<Material> material) {
        return make_shared<Mesh>( Mesh(geometry, material) );
    }
    
    Mesh::Mesh()
    {}
    
    Mesh::Mesh( ptr<Geometry> geometry, ptr<Material> material ) :
        geometry( geometry ),
        material( material )
    {}
    
    Mesh::~Mesh()
    {}
    
    void Mesh::draw(ptr<Shader> shader, bool gamma ) {
        
        glPolygonMode( GL_FRONT_AND_BACK, material->wireframe ? GL_LINE : GL_FILL );
        glLineWidth( material->wireframeLinewidth );
        
        if( material->wrapAround )
            shader->setUniform("wrapRGB", material->wrapRGB );
        
        if( texture != nullptr ) {
            if( instance_of(texture, EnvMap)) {
                glCullFace( GL_BACK );
                glFrontFace( GL_CW );
                
                glActiveTexture( GL_TEXTURE0 );
                glBindTexture( GL_TEXTURE_2D, texture->textureID );
                shader->setUniform("map", 0);
            }
            else {
                glActiveTexture( GL_TEXTURE0 );
                glBindTexture( GL_TEXTURE_2D, texture->textureID );
                shader->setUniform("map", 0);
            }
        }
        
        if( normalMap != nullptr ) {
            glActiveTexture( GL_TEXTURE1 );
            glBindTexture( GL_TEXTURE_2D, normalMap->textureID );
            shader->setUniform("normal_map", 1);
        }
        
        if( specularMap != nullptr ) {
            glActiveTexture( GL_TEXTURE2 );
            glBindTexture( GL_TEXTURE_2D, specularMap->textureID );
            shader->setUniform( "specular_map" , 2 );
        }
        
        if( envMap != nullptr ) {
            glCullFace( GL_BACK );
            glFrontFace( GL_CW );
            
            glActiveTexture( GL_TEXTURE3 );
            glBindTexture( GL_TEXTURE_CUBE_MAP, envMap->textureID );
            shader->setUniform( "env_map", 3 );
        }
        
        
        ptr<MeshPhongMaterial> phong_material;
        if( instance_of(material, MeshPhongMaterial )) {
            phong_material = downcast(material, MeshPhongMaterial );
            
            shader->setUniform( "opacity",   phong_material->opacity );
            shader->setUniform( "emissive",  phong_material->emissive, 1.0, gamma );
            shader->setUniform( "ambient",   phong_material->ambient, 1.0, gamma );
            shader->setUniform( "diffuse",   phong_material->color, 1.0, gamma );
            shader->setUniform( "specular",  phong_material->specular, 1.0, gamma );
            shader->setUniform( "shininess", phong_material->shininess );
            shader->setUniform( "model_mat", matrixWorld );

            
            
            
            if( this->envMap != nullptr ) {
                shader->setUniform( "combine",          phong_material->combine );
                shader->setUniform( "reflectivity",     (GLfloat) 1.0 ); //phong_material->reflectivity );
                shader->setUniform( "refraction_ratio", (GLfloat) 0.0 ); //phong_material->refractionRatio );
                shader->setUniform( "flip_env_map",     (GLfloat)-1.0 );
                shader->setUniform( "use_refraction",   false );
            }
        }
        
        
        /* Vertices */
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, geometry->bufferIDs[0] );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Normals */
        glEnableVertexAttribArray( 1 );
        glBindBuffer( GL_ARRAY_BUFFER, geometry->bufferIDs[1] );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* UVs */
        glEnableVertexAttribArray( 2 );
        glBindBuffer( GL_ARRAY_BUFFER, geometry->bufferIDs[3] );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Indices */
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometry->bufferIDs[2] );
        glDrawElements( GL_TRIANGLES, geometry->noOfElements, GL_UNSIGNED_SHORT, (void*) 0);
        
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 2 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}