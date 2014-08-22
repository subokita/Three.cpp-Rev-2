//
//  PolyhedronGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PolyhedronGeometry.h"
#include "Face3.h"
#include "MathUtils.h"

using std::vector;
using std::initializer_list;

namespace three {
    
    PolyhedronGeometry::PolyhedronGeometry() {
        
    }
    
    PolyhedronGeometry::PolyhedronGeometry( initializer_list<glm::vec3> vertices, initializer_list<int> indices,
                                            float radius, int detail ) :
        radius( radius ),
        detail( detail )
    {
        init( vertices, indices );
    }
    
    PolyhedronGeometry::~PolyhedronGeometry() {
        
    }
    
    void PolyhedronGeometry::init(initializer_list<glm::vec3>& vertices, initializer_list<int>& indices) {
        for( glm::vec3 vertex: vertices )
            prepare( vertex );
        
        for( int i = 0; i < indices.size(); i+= 3 ) {
            TempVec v1 = vecs[ *(indices.begin()+i)   ];
            TempVec v2 = vecs[ *(indices.begin()+i+1) ];
            TempVec v3 = vecs[ *(indices.begin()+i+2) ];
            
            ptr<Face3> face = Face3::create(v1.index, v2.index, v3.index);
            face->setVertexNormals({ v1.vertex, v2.vertex, v3.vertex });
            this->faces.push_back(face);
        }
        
        /* Be carefule, since subdivide add more faces in */
        int current_faces_len = static_cast<int>(faces.size());
        for( int i = 0; i < current_faces_len; i++ )
            subdivide(faces[i], detail);
        
        for( int i = 0; i < faces.size(); i++ ) {
            float x0 = faces[i]->uvs[0].x;
            float x1 = faces[i]->uvs[1].x;
            float x2 = faces[i]->uvs[2].x;
            
            
            float max = std::max( {x0, x1, x2} );
            float min = std::min( {x0, x1, x2} );
            
            if( max > 0.9 && min < 0.1 ) {
                if( x0 < 0.2 )
                    faces[i]->uvs[0].x += 1.0;
                if( x1 < 0.2 )
                    faces[i]->uvs[1].x += 1.0;
                if( x2 < 0.2 )
                    faces[i]->uvs[2].x += 1.0;
            }
        }
        
        for( TempVec& vec: vecs )
            this->vertices.push_back(vec.vertex * radius);
        
        vecs.clear();
    }
    
    TempVec PolyhedronGeometry::prepare( glm::vec3 vector ) {
        TempVec vertex;
        vertex.index    = static_cast<int>(vecs.size());
        vertex.vertex   = glm::normalize(vector);
        vertex.uv[0]    = azimuth( vector ) / 2.0 / M_PI + 0.5;
        vertex.uv[1]    = 1.0 - (inclination( vector ) / M_PI + 0.5);
        
        vecs.push_back( vertex );
        return vertex;
    }
    
    void PolyhedronGeometry::subdivide(ptr<Face3> face, int detail) {
        float cols  = powf(2, detail);

        TempVec a = prepare( vecs[face->a].vertex );
        TempVec b = prepare( vecs[face->b].vertex );
        TempVec c = prepare( vecs[face->c].vertex );

        vector2D<TempVec> v( cols+1, vector<TempVec>(cols+1, TempVec()) );

        for( int i = 0; i <= cols; i++ ) {
            TempVec aj = prepare( MathUtils::lerp( a.vertex, c.vertex, i / cols ) );
            TempVec bj = prepare( MathUtils::lerp( b.vertex, c.vertex, i / cols ) );
            
            float rows = cols - i;
            
            for( int j = 0; j <= rows; j++ ) {
                if( j == 0 && i == cols ) {
                    v[i][j] = aj;
                }
                else {
                    v[i][j] = prepare( MathUtils::lerp( aj.vertex, bj.vertex, j / rows) );
                }
            }
        }

        for( int i = 0; i < cols; i++ ) {
            
            for( int j = 0; j < 2 * (cols - i) - 1; j++ ) {
                int k = static_cast<int>(floorf( j / 2.0));
                
                if( j % 2 == 0)
                    make( v[i][k+1], v[i+1][k], v[i][k] );
                else
                    make( v[i][k+1], v[i+1][k+1], v[i+1][k] );
            }
        }
    }
    
    
    void PolyhedronGeometry::make( TempVec& v1, TempVec& v2, TempVec& v3 ) {
        ptr<Face3> face = Face3::create(v1.index, v2.index, v3.index);
        face->setVertexNormals({ v1.vertex, v2.vertex, v3.vertex });
        faces.push_back( face );
        
        glm::vec3 centroid = v1.vertex + v2.vertex + v3.vertex;
        centroid /= 3.0;
        
        float azi = azimuth(centroid);
        
        face->setVertexUVs({
            correctUV( v1.uv, v1.vertex, azi ),
            correctUV( v2.uv, v2.vertex, azi ),
            correctUV( v3.uv, v3.vertex, azi )
        });
    }
    
    float PolyhedronGeometry::azimuth( glm::vec3& vector ) {
        return atan2f( vector.z, -vector.x );
    }
    
    float PolyhedronGeometry::inclination( glm::vec3& vector ) {
        return atan2f( -vector.y, sqrtf( vector.x * vector.x + vector.z * vector.z ) );
    }
    
    
    glm::vec2 PolyhedronGeometry::correctUV( glm::vec2& uv, glm::vec3& vector, float azimuth ) {
        glm::vec2 result = uv;
        
        if( (azimuth < 0.0) && (uv.x == 1.0) )
            result = glm::vec2( uv.x -1, uv.y );
        
        if( (vector.x == 0.0) && (vector.z == 0.0) )
            result = glm::vec2( azimuth / 2.0 / M_PI + 0.5, uv.y );
        
        return result;
    }
}