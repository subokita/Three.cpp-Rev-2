//
//  ShadowMapPlugin.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 24/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ShadowMapPlugin.h"

#include "Camera.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "Scene.h"
#include "Shader.h"
#include "ShaderLib.h"
#include "ShaderLibraries.h"
#include "ShadowTexture.h"
#include "Projector.h"
#include "VirtualLight.h"
#include "Gyroscope.h"

namespace three {
    ptr<ShadowMapPlugin> ShadowMapPlugin::create() {
        return make_shared<ShadowMapPlugin>();
    }
    
    ShadowMapPlugin::ShadowMapPlugin() :
        frustum         (Frustum::create()),
        depthShader     (SHADERLIB_DEPTH_RGBA)
    {
    }
    
    ShadowMapPlugin::~ShadowMapPlugin() {
    }
    
    void ShadowMapPlugin::init( ptr<Scene> scene, ptr<Camera> camera ) {
        depthShader->compileShader();
        
        descendants = scene->getDescendants();
        std::sort(descendants.begin(), descendants.end(), [](ptr<Object3D> a, ptr<Object3D> b){
            return a->position.z < b->position.z;
        });
        
        for( auto entry: scene->getDirectionalLights().getCollection() ) {
            ptr<DirectionalLight> light = entry.second;
            if( !light->castShadow )
                continue;
            
            if( light->shadowCascade ) {
                throw runtime_error( "Cascaded shadow is not implemented at this moment" );
//                /* Use shadow cascade */
//                for(int i = 0; i < light->shadowCascadeArray.size(); i++ ) {
//                    ptr<VirtualLight> virtual_light;
//                    
//                    if( light->shadowCascadeArray[i] == nullptr  ) {
//                        virtual_light = createVirtualLight( light, i );
//                        virtual_light->originalCamera = camera;
//                        
//                        
//                        /* FIXME: Ummm, what's the point of this? */
//                        ptr<Gyroscope> gyro = Gyroscope::create();
//                        gyro->position = light->shadowCascadeOffset;
//                        gyro->add   ( virtual_light );
//                        gyro->add   ( virtual_light->target );
//                        camera->add ( gyro );
//                        
//                        light->shadowCascadeArray[i] = virtual_light;
//                    }
//                    else {
//                        virtual_light = light->shadowCascadeArray[i];
//                    }
//                    
//                    updateVirtualLight(light, i );
//                    lights.push_back( virtual_light );
//                }
            }
            else {
                lights.push_back( light );
            }
        }
        
        for( auto entry: scene->getSpotLights().getCollection() ) {
            ptr<SpotLight> light = entry.second;
            if( light->castShadow )
                lights.push_back( light );
        }
        
        
        const Rect viewport_size = scene->getViewportSize();

        for( ptr<Light> light: lights ) {
            if( light->shadowMap == nullptr ) {
                FILTER shadow_filter = FILTER::LINEAR;
                
                if( scene->getShadowMapType() == SHADOW_MAP::PCF_SOFT )
                    shadow_filter = FILTER::NEAREST;
                
                light->shadowMap     = RenderTarget::create(GL_FRAMEBUFFER);
                light->shadowTexture = ShadowTexture::create(GL_CLAMP, GL_CLAMP, GL_CLAMP, static_cast<GLuint>(shadow_filter), static_cast<GLuint>(shadow_filter));
                
                light->shadowMap->generateFrameBuffer();
                light->shadowTexture->genTexture();

                light->shadowMap->bind();
                light->shadowTexture->bind();

                
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, light->shadowMapSize.x, light->shadowMapSize.y, 0, GL_RGBA, GL_FLOAT, 0 );

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(shadow_filter) );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(shadow_filter) );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
                
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, light->shadowTexture->textureID, 0 );
                glDrawBuffer( GL_COLOR_ATTACHMENT0 );
                glReadBuffer( GL_NONE );
                
                light->shadowMap->unbind();

            }
            
            if( light->shadowCamera == nullptr ) {
                if( instance_of(light, SpotLight)) {
                    light->shadowCamera = PerspectiveCamera::create(light->shadowCameraFOV,
                                                                    light->shadowMapSize.x / light->shadowMapSize.y,
                                                                    light->shadowCameraNear,
                                                                    light->shadowCameraFar );
                }
                else if( instance_of(light, DirectionalLight)) {
                    light->shadowCamera = OrthographicCamera::create(light->shadowCameraLeft,
                                                                     light->shadowCameraRight,
                                                                     light->shadowCameraTop,
                                                                     light->shadowCameraBottom,
                                                                     light->shadowCameraNear,
                                                                     light->shadowCameraFar );
                }
                else {
                    cerr << "Unsupported light type for shadow" << endl;
                    continue;
                }
            }
            
//            if( light->shadowCameraVisible && !light->cameraHelper ) {
//                
//            }
//            
//            if( instance_of(light, VirtualLight) ) {
//                ptr<VirtualLight> vlight = downcast(light, VirtualLight );
//                if( vlight->originalCamera == camera )
//                    updateShadowCamera(camera, vlight );
//            }
        }
        
        
        #ifdef DEBUG_SHADOW
        
        passthruShader = SHADERLIB_SIMPLE_PASS->clone();
        passthruShader->compileShader();
        
        GLuint quad_vertex_array;
        glGenVertexArrays( 1, &quad_vertex_array );
        glBindVertexArray( quad_vertex_array );
        
        static const GLfloat quad_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
        };
        
        
        glGenBuffers( 1, &quad_vertex_buffer );
        glBindBuffer( GL_ARRAY_BUFFER, quad_vertex_buffer );
        glBufferData( GL_ARRAY_BUFFER, sizeof( quad_vertex_buffer_data ), quad_vertex_buffer_data, GL_STATIC_DRAW );
        #endif
    }
    
    void ShadowMapPlugin::setState( ptr<Scene> scene, ptr<Camera> camera ) {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glDisable( GL_BLEND );
        glEnable( GL_DEPTH_TEST );
    }
    
    void ShadowMapPlugin::render( ptr<Scene> scene, ptr<Camera> camera ) {
        const Rect viewport = scene->getViewportSize();
        
        for( ptr<Light> light: lights ) {
            light->updateMatrixWorld(false);

            auto shadow_map = light->shadowMap;
            auto shadow_cam = light->shadowCamera;
            
            shadow_cam->position = light->position;
            shadow_cam->lookAt( light->target->position );

            shadow_cam->updateMatrixWorld(false);
            glm::mat4 vp_mat = shadow_cam->getProjectionMatrix() * shadow_cam->matrixWorld;
            
            light->shadowMatrix = SHADOW_BIAS_MATRIX * vp_mat;
            light->shadowMap->bind();
            frustum->setFrom( vp_mat );
            
            glViewport(0, 0, light->shadowMapSize[0], light->shadowMapSize[1]);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            
            depthShader->bind();
            for( auto object: descendants ){
                if( instance_of(object, Mesh) == false )
                    continue;

                if( object->castShadow == false )
                    continue;
                
                if( frustum->intersects(object) == false )
                    continue;
                
                
                depthShader->draw(shadow_cam, nullptr, object, false );
            }
            depthShader->unbind();
        }
     
    }
    
    
    void ShadowMapPlugin::debugShadow() {
        #ifdef DEBUG_SHADOW
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glClearColor(1.0, 1.0, 1.0, 1.0);
        
        for( int i = 0; i < lights.size(); i++ ) {
            auto light = lights[i];
            
            glViewport(0 + i * 100, 0, 100, 100);
            passthruShader->bind();
            
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, light->shadowTexture->textureID );
            passthruShader->getShader()->setUniform("texture_sampler", 0);
            
            glEnableVertexAttribArray( 0 );
            glBindBuffer( GL_ARRAY_BUFFER, quad_vertex_buffer );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
            glDrawArrays( GL_TRIANGLES, 0, 6 );
            
            glDisableVertexAttribArray( 0 );
            passthruShader->unbind();
        }
        #endif
    }
    
    
//    ptr<VirtualLight> ShadowMapPlugin::createVirtualLight( ptr<Light> light, int cascade ) {
//        ptr<VirtualLight> virtual_light = VirtualLight::create();
//        
//        virtual_light->isVirtual  = true;
//        virtual_light->onlyShadow = true;
//        virtual_light->castShadow = true;
//        
//        virtual_light->shadowCameraNear     = light->shadowCameraNear;
//        virtual_light->shadowCameraFar      = light->shadowCameraFar;
//        virtual_light->shadowCameraTop      = light->shadowCameraTop;
//        virtual_light->shadowCameraBottom   = light->shadowCameraBottom;
//        virtual_light->shadowCameraLeft     = light->shadowCameraLeft;
//        virtual_light->shadowCameraRight    = light->shadowCameraRight;
//        
//        virtual_light->shadowCameraVisible  = light->shadowCameraVisible;
//        virtual_light->shadowDarkness       = light->shadowDarkness;
//        
//        virtual_light->shadowBias           = light->shadowCascadeBias[cascade];
//        virtual_light->shadowMapSize.x      = light->shadowCascadeWidth[cascade];
//        virtual_light->shadowMapSize.y      = light->shadowCascadeHeight[cascade];
//        
//        float near_z = light->shadowCascadeNearZ[cascade];
//        float far_z  = light->shadowCascadeFarZ[cascade];
//        
//        virtual_light->pointsFrustum[0] = glm::vec3(-1.0, -1.0, near_z);
//        virtual_light->pointsFrustum[1] = glm::vec3( 1.0, -1.0, near_z);
//        virtual_light->pointsFrustum[2] = glm::vec3(-1.0,  1.0, near_z);
//        virtual_light->pointsFrustum[3] = glm::vec3( 1.0,  1.0, near_z);
//        
//        virtual_light->pointsFrustum[4] = glm::vec3(-1.0, -1.0, far_z);
//        virtual_light->pointsFrustum[5] = glm::vec3( 1.0, -1.0, far_z);
//        virtual_light->pointsFrustum[6] = glm::vec3(-1.0,  1.0, far_z);
//        virtual_light->pointsFrustum[7] = glm::vec3( 1.0,  1.0, far_z);
//        
//        return virtual_light;
//    }
//    
//    void ShadowMapPlugin::updateVirtualLight( ptr<Light> light, int cascade ) {
//        ptr<VirtualLight> virtual_light = light->shadowCascadeArray[cascade];
//        
//        virtual_light->position         = light->position;
//        virtual_light->target->position = light->target->position ;
//        
//        virtual_light->lookAt( virtual_light->target->position );
//        
//        virtual_light->shadowCameraVisible  = light->shadowCameraVisible;
//        virtual_light->shadowDarkness       = light->shadowDarkness;
//        virtual_light->shadowBias           = light->shadowCascadeBias[cascade];
//        
//        float near_z = light->shadowCascadeNearZ[cascade];
//        float far_z  = light->shadowCascadeFarZ[cascade];
//        
//        virtual_light->pointsFrustum[0].z = near_z;
//        virtual_light->pointsFrustum[1].z = near_z;
//        virtual_light->pointsFrustum[2].z = near_z;
//        virtual_light->pointsFrustum[3].z = near_z;
//        
//        virtual_light->pointsFrustum[4].z = far_z;
//        virtual_light->pointsFrustum[5].z = far_z;
//        virtual_light->pointsFrustum[6].z = far_z;
//        virtual_light->pointsFrustum[7].z = far_z;
//    }
//    
//    void ShadowMapPlugin::updateShadowCamera( ptr<Camera> camera, ptr<VirtualLight> light ) {
//        glm::vec3 min_vec( MAX_FLOAT, MAX_FLOAT, MAX_FLOAT );
//        glm::vec3 max_vec( MIN_FLOAT, MIN_FLOAT, MIN_FLOAT );
//        
//        ptr<OrthographicCamera> shadow_cam = downcast(light->shadowCamera, OrthographicCamera);
//        
//        for( int i = 0; i < 8; i++ ) {
//            glm::vec4 point = glm::vec4(light->pointsWorld[i], 1.0);
//            
//            point = Projector::unprojectVector(point, camera);
//            point = shadow_cam->matrixWorldInverse * point;
//            
//            if( point.x < min_vec.x )
//                min_vec.x = point.x;
//            if( point.x > max_vec.x )
//                max_vec.x = point.x;
//            
//            
//            if( point.y < min_vec.y )
//                min_vec.y = point.y;
//            if( point.y > max_vec.y )
//                max_vec.y = point.y;
//            
//            
//            if( point.z < min_vec.z )
//                min_vec.z = point.z;
//            if( point.z > max_vec.z )
//                max_vec.z = point.z;
//        }
//        
//        shadow_cam->left  = min_vec.x;
//        shadow_cam->right = max_vec.x;
//        shadow_cam->bottom = min_vec.y;
//        shadow_cam->top    = max_vec.y;
//        shadow_cam->updateProjectionMatrix();
//    }
    
}