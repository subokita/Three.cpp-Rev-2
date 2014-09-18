//
//  Mesh.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Mesh.h"
#include "Shader.h"
#include "ShaderLib.h"
#include "Geometry.h"
#include "Material.h"
#include "NormalMap.h"
#include "SpecularMap.h"
#include "Face3.h"
#include "EnvMap.h"
#include "Box3.h"

namespace three {
    
    ptr<Mesh> Mesh::create(ptr<Geometry> geometry, ptr<Material> material) {
        ptr<Mesh> mesh = make_shared<Mesh>();
        mesh->setGeometry( geometry );
        mesh->setMaterial( material );
        return mesh;
    }
    
    Mesh::Mesh():
        glBuffersInitialized(false)
    {}
    
    //FIXME: Can't remember why I commented this one out.
//    Mesh::Mesh( ptr<Geometry> geometry, ptr<Material> material ) :
//        geometry            ( geometry ),
//        material            ( material ),
//        glBuffersInitialized(false)
//    {}
    
    Mesh::~Mesh() {
        if( glBuffersInitialized )
            glDeleteBuffers(5, bufferIDs );
    }
    
    
    void Mesh::setPointMode( bool flag ) {
        if( this->material != nullptr ) {
            if( flag ) {
                this->material->setSide(SIDE::DOUBLE_SIDE);
                this->material->setPolygonMode(POLYGON_MODE::POINT);
            }
            else {
                this->material->setSide(SIDE::FRONT_SIDE);
                this->material->setPolygonMode(POLYGON_MODE::POLYGON);
            }
        }
    }
    
    void Mesh::setWireframeMode( bool flag ) {
        if( this->material != nullptr ) {
            if( flag ){
                this->material->setSide(SIDE::DOUBLE_SIDE);
                this->material->setPolygonMode(POLYGON_MODE::WIREFRAME);
            }
            else {
                this->material->setSide(SIDE::FRONT_SIDE);
                this->material->setPolygonMode(POLYGON_MODE::POLYGON);
            }
        }
    }
    
    bool Mesh::hasTexture(){
        return texture != nullptr;
    }
    
    bool Mesh::hasNormalMap() {
        return normalMap != nullptr;
    }
    
    bool Mesh::hasSpecularMap() {
        return specularMap != nullptr;
    }
    
    bool Mesh::hasEnvMap() {
        return envMap != nullptr;
    }
    
    bool Mesh::hasGeometry() {
        return geometry != nullptr;
    }
    
    bool Mesh::hasMaterial() {
        return material != nullptr;
    }
    
    bool Mesh::areGLBuffersInitialized() {
        return this->glBuffersInitialized;
    }
    
    const ptr<Texture> Mesh::getTexture() {
        return this->texture;
    }
    
    const ptr<NormalMap> Mesh::getNormalMap() {
        return this->normalMap;
    }
    
    const ptr<SpecularMap> Mesh::getSpecularMap() {
        return this->specularMap;
    }
    
    const ptr<EnvMap> Mesh::getEnvMap() {
        return this->envMap;
    }
    
    const ptr<Geometry> Mesh::getGeometry() {
        return this->geometry;
    }
    
    const ptr<Material> Mesh::getMaterial() {
        return this->material;
    }
    
    void Mesh::setTexture( const ptr<Texture> texture ) {
        this->texture = texture;
    }
    
    void Mesh::setNormalMap( const ptr<NormalMap> normal_map ) {
        this->normalMap = normal_map;
    }
    
    void Mesh::setSpecularMap( const ptr<SpecularMap> specular_map ) {
        this->specularMap = specular_map;
    }
    
    void Mesh::setEnvMap( const ptr<Texture> env_map ) {
        if( !instance_of(env_map, EnvMap)) {
            throw runtime_error( "Error, trying to set non-env map as env-map of a mesh" );
        }
        this->envMap = downcast(env_map, EnvMap);
    }
    
    void Mesh::setEnvMap( const ptr<EnvMap> env_map ) {
        this->envMap = env_map;
    }
    
    void Mesh::setGeometry( const ptr<Geometry> geometry ) {
        if( this->geometry != nullptr ) {
            this->geometry->setParent( nullptr );
            this->remove( this->geometry );
        }
        
        this->geometry = geometry;
        this->add( geometry );
    }
    
    void Mesh::setMaterial( const ptr<Material> material ) {
        this->material = material;
    }
    
    void Mesh::setUniforms(ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
        shader->setUniform( "model_mat", matrixWorld );
        
        if( material->doesWrapAround() )
            shader->setUniform("wrapRGB", material->getWrapRGB() );
        
        if( texture != nullptr )
            texture->setUniforms(shader_lib, gamma );
        
        if( normalMap != nullptr )
            normalMap->setUniforms( shader_lib, gamma );
        
        if( specularMap != nullptr )
            specularMap->setUniforms( shader_lib, gamma );
        
        if( envMap != nullptr )
            envMap->setUniforms( shader_lib, gamma );
        
        
        material->setUniforms( shader_lib, gamma );
    }
    
    void Mesh::initGLBuffers() {
        geometry->noOfElements = static_cast<int>( geometry->faces.size() * 3);
        
        std::vector<unsigned short> index;
        std::vector<glm::vec3> normals( geometry->vertices.size() );
        std::vector<glm::vec2> uvs    ( geometry->vertices.size() );
        std::vector<glm::vec3> colors ( geometry->vertices.size() );
        
        for( ptr<Face3> face: geometry->faces ) {
            index.push_back( face->a );
            index.push_back( face->b );
            index.push_back( face->c );
            
            normals[face->a] = face->vertexNormals[0];
            normals[face->b] = face->vertexNormals[1];
            normals[face->c] = face->vertexNormals[2];
            
            uvs[face->a] = face->uvs[0];
            uvs[face->b] = face->uvs[1];
            uvs[face->c] = face->uvs[2];

            colors[face->a] = face->vertexColors[0];
            colors[face->b] = face->vertexColors[1];
            colors[face->c] = face->vertexColors[2];
        }
        
        geometry->updateMatrixWorld(true);
        for( auto& vert: geometry->vertices)
            vert = glm::vec3(geometry->matrix * glm::vec4(vert, 1.0));
        
        for( auto& normal: normals)
            normal = glm::vec3(geometry->matrix * glm::vec4(normal, 1.0));
        
        glGenBuffers( 5, bufferIDs );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * geometry->vertices.size(), &geometry->vertices[0], GL_STATIC_DRAW );
        
        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * normals.size(), &normals[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * geometry->faces.size() * 3, &index[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * uvs.size(), &uvs[0], GL_STATIC_DRAW );

        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[4] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * colors.size(), &colors[0], GL_STATIC_DRAW );
        
        glBuffersInitialized = true;
        
        boundingBox = Box3::create();
        boundingBox->setFrom( geometry->vertices );
        
        geometry->vertices.clear();
        geometry->faces.clear();
    }
    
    void Mesh::setGLState() {
        glPolygonMode( GL_FRONT_AND_BACK, static_cast<GLint>(material->getPolygonMode()) );
        material->setGLState();
    }
    
    void Mesh::draw() {
        /* Vertices */
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Normals */
        glEnableVertexAttribArray( 1 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* UVs */
        glEnableVertexAttribArray( 2 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Colors */
        glEnableVertexAttribArray( 3 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[4] );
        glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Indices */
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
        glDrawElements( GL_TRIANGLES, geometry->noOfElements, GL_UNSIGNED_SHORT, (void*) 0);
        
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 2 );
        glDisableVertexAttribArray( 3 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}