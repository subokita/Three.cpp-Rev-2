//
//  CylinderGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 18/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "CylinderGeometry.h"
#include "Face3.h"

using std::vector;

namespace three {
    ptr<CylinderGeometry> CylinderGeometry::create(const float radius_top, const float radius_bottom,
                                                   const float height, const int radial_segments,
                                                   const int height_segments, const bool open_ended){
        
        return make_shared<CylinderGeometry>(CylinderGeometry( radius_top, radius_bottom, height,
                                                               radial_segments, height_segments, open_ended ));
    }
    
    CylinderGeometry::~CylinderGeometry(){}
    
    CylinderGeometry::CylinderGeometry():
        radiusTop     (0.5),
        radiusBottom  (0.5),
        height        (1.0),
        radialSegments(8),
        heightSegments(1),
        openEnded     (false)
    {
        init();
    }
    
    CylinderGeometry::CylinderGeometry(const float radius_top, const float radius_bottom, const float height,
                                       const int radial_segments, const int height_segments, const bool open_ended):
        radiusTop     (radius_top),
        radiusBottom  (radius_bottom),
        height        (height),
        radialSegments(radial_segments),
        heightSegments(height_segments),
        openEnded     (open_ended)
    {
        init();
    }
    
    void CylinderGeometry::init(){
        float half_height = height / 2.0;
        
        vertices.clear();
        
        vector2D<uint> verts_count( heightSegments + 1, vector<uint>(radialSegments + 1, 0) );
        vector2D<glm::vec2> uvs;
        
        for( int y = 0; y < heightSegments + 1; y++ ) {
            
            float v = 1.0 * y / heightSegments;
            float radius = v * ( radiusBottom - radiusTop ) + radiusTop;
            
            vector<uint> verts_row;
            vector<glm::vec2> uvs_row;
            
            for( int x = 0; x < radialSegments + 1; x++ ) {
                float u = 1.0 * x / radialSegments;
                
                glm::vec3 vertex(
                     radius * sinf( u * M_PI * 2.0 ),
                     -v * height + half_height,
                     radius * cosf( u * M_PI * 2.0 )
                );
                
                vertices.push_back( vertex );
                
                verts_count[y][x] = static_cast<uint>(this->vertices.size() - 1);
                uvs_row.push_back( glm::vec2(u, 1.0 - v) );
            }
            
            verts_count.push_back( verts_row );
            uvs.push_back( uvs_row );
        }
        
        float tan_theta = (radiusBottom - radiusTop) / height;
        
        glm::vec3 na, nb;
        
        for( int x = 0; x < radialSegments; x++ ) {
            if( radiusTop != 0.0 ) {
                na = vertices[ verts_count[0][x  ] ];
                nb = vertices[ verts_count[0][x+1] ];
            }
            else {
                na = vertices[ verts_count[1][x  ] ];
                nb = vertices[ verts_count[1][x+1] ];
            }
            
            na.y = sqrt(na.x * na.x + na.z * na.z) * tan_theta;
            na = glm::normalize( na );
            
            nb.y = sqrt(nb.x * nb.x + nb.z * nb.z) * tan_theta;
            nb = glm::normalize( nb );
            
            for (int y = 0; y < heightSegments; y++ ) {
                int face_index_1 = verts_count[y  ][x  ];
                int face_index_2 = verts_count[y+1][x  ];
                int face_index_3 = verts_count[y+1][x+1];
                int face_index_4 = verts_count[y  ][x+1];
                
                glm::vec3 n1 = na;
                glm::vec3 n2 = na;
                glm::vec3 n3 = nb;
                glm::vec3 n4 = nb;
                
                glm::vec2 uv1 = uvs[y  ][x  ];
                glm::vec2 uv2 = uvs[y+1][x  ];
                glm::vec2 uv3 = uvs[y+1][x+1];
                glm::vec2 uv4 = uvs[y  ][x+1];
                
                ptr<Face3> face = Face3::create( face_index_1, face_index_2, face_index_4 );
                face->setVertexNormals( {n1, n2, n4} );
                face->setVertexUVs({uv1, uv2, uv4});
                faces.push_back( face );
                
                face = Face3::create( face_index_2, face_index_3, face_index_4 );
                face->setVertexNormals({n2, n3, n4});
                face->setVertexUVs( {uv2, uv3, uv4} );
                faces.push_back( face );
            }
        }
        
        if( !openEnded  ) {
            if( radiusTop > 0.0 ) {
                vertices.push_back( glm::vec3(0.0, half_height, 0.0) );
                
                glm::vec3 normal(0.0, 1.0, 0.0);
                
                for( int x = 0; x < radialSegments ; x++ ) {
                    int v1 = verts_count[0][x  ];
                    int v2 = verts_count[0][x+1];
                    int v3 = static_cast<int>(vertices.size() - 1);
                    
                    glm::vec2 uv1 = uvs[0][x  ];
                    glm::vec2 uv2 = uvs[0][x+1];
                    glm::vec2 uv3 = glm::vec2( uv2.x, 0.0 );
                    
                    ptr<Face3> face = Face3::create( v1, v2, v3 );
                    face->setVertexNormals( {normal, normal, normal});
                    face->setVertexUVs({uv1, uv2, uv3});
                    faces.push_back( face );
                }
            }
            
            if( radiusBottom > 0.0 ) {
                vertices.push_back( glm::vec3(0.0, -half_height, 0.0) );
                
                glm::vec3 normal(0.0, -1.0, 0.0);
                
                for( int x = 0; x < radialSegments ; x++ ) {
                    int v1 = verts_count[heightSegments][x+1];
                    int v2 = verts_count[heightSegments][x  ];
                    int v3 = static_cast<int>(vertices.size() - 1);
                    
                    glm::vec2 uv1 = uvs[heightSegments][x+1];
                    glm::vec2 uv2 = uvs[heightSegments][x  ];
                    glm::vec2 uv3 = glm::vec2( uv2.x, 1.0 );
                    
                    ptr<Face3> face = Face3::create( v1, v2, v3 );
                    face->setVertexNormals( {normal, normal, normal});
                    face->setVertexUVs({uv1, uv2, uv3});
                    faces.push_back( face );
                }
            }
        }
        
        computeVertexNormals(true);
        computeFaceNormals();
    }
}