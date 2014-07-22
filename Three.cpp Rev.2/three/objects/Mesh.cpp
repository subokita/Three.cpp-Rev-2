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
    
    Mesh::~Mesh() {
    }
    
    
    void Mesh::setUniforms(ptr<Shader> shader, bool gamma ) {
        
        shader->setUniform( "model_mat", matrixWorld );
        
        if( material->wrapAround )
            shader->setUniform("wrapRGB", material->wrapRGB );
        
        if( texture != nullptr )
            texture->setUniforms(shader, gamma );
        
        if( normalMap != nullptr )
            normalMap->setUniforms( shader, gamma );
        
        if( specularMap != nullptr )
            specularMap->setUniforms( shader, gamma );
        
        if( envMap != nullptr )
            envMap->setUniforms( shader, gamma );
        
        
        material->setUniforms( shader, gamma );
    }
    
    void Mesh::draw() {
        
        glPolygonMode( GL_FRONT_AND_BACK, material->wireframe ? GL_LINE : GL_FILL );
        glLineWidth( material->wireframeLinewidth );
        
        if( material->side == three::DoubleSide )  {
            glDisable( GL_CULL_FACE );
        }
        else {
            glEnable( GL_CULL_FACE );
            glCullFace( GL_BACK );
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