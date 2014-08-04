//
//  Loader.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Loader.h"
#include "three.h"

using namespace std;
namespace three {
    
    ptr<Mesh> Loader::loadPLY( const std::string path, const std::string filename ) {
        cout << path << "/" << filename << endl;
        
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile( path + "/" + filename, aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals );
        
        if( !scene ) {
            throw runtime_error(importer.GetErrorString());
        }
        
        if( scene->HasMeshes() == false ) {
            throw runtime_error( "No mesh found in " + path + "/" + filename );
        }
        
        ptr<Geometry> geometry = Geometry::create();
        
        
        bool has_vertex_normals = false;
        for( int m = 0; m < scene->mNumMeshes; m++ ) {
            const aiMesh * mesh = scene->mMeshes[m];
            
            if( mesh->HasFaces() ) {
            
                
                for( int i = 0; i < mesh->mNumVertices; i++ )
                    geometry->addVertex( toGLMVec3(mesh->mVertices[i]) );
                
                for( int i = 0; i < mesh->mNumFaces; i++ ) {
                    aiFace aiface = mesh->mFaces[i];
                    
                    ptr<Face3> face = Face3::create(aiface.mIndices[0],
                                                    aiface.mIndices[1],
                                                    aiface.mIndices[2]);
                    
                    if( mesh->HasNormals() ) {
                        has_vertex_normals = true;
                        face->setVertexNormals({
                            toGLMVec3(mesh->mNormals[face->a]),
                            toGLMVec3(mesh->mNormals[face->b]),
                            toGLMVec3(mesh->mNormals[face->c]),
                        });
                    }
                    
                    if( mesh->HasTextureCoords(0) ) {
                        face->setVertexUVs({
                            toGLMVec2( mesh->mTextureCoords[0][face->a] ),
                            toGLMVec2( mesh->mTextureCoords[0][face->b] ),
                            toGLMVec2( mesh->mTextureCoords[0][face->c] )
                        });
                    }

                    geometry->addFace( face );
                }
            }
        }
        
        if( !has_vertex_normals )
            geometry->computeVertexNormals(true);
        
        return Mesh::create(geometry, PhongMaterial::create());
    }
 
    
    glm::vec2 Loader::toGLMVec2(aiVector3D& ai_vector) {
        return glm::vec2( ai_vector.x, ai_vector.y );
    }
    glm::vec3 Loader::toGLMVec3(aiVector3D& ai_vector) {
        return glm::vec3( ai_vector.x, ai_vector.y, ai_vector.z );
    }
}