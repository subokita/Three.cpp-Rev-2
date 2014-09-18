//
//  ShaderLib.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 20/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ShaderLib.h"
#include "three.h"
#include <sstream>

using std::stringstream;

namespace three {
    ptr<ShaderLib> ShaderLib::create( ptr<Mesh> mesh ) {
        /* Return appropriate shader libs according to material type */
        if( instance_of(mesh->getMaterial(), PhongMaterial) )
            return SHADERLIB_PHONG->clone();
        
        else if ( instance_of(mesh->getMaterial(), MeshCubeMapMaterial ) )
            return SHADERLIB_CUBEMAP->clone();
        
        else if ( instance_of(mesh->getMaterial(), BasicMaterial ) ||
                  instance_of(mesh->getMaterial(), LineBasicMaterial ) )
            return SHADERLIB_BASIC->clone();
        
        else if ( instance_of(mesh->getMaterial(), LambertMaterial))
            return SHADERLIB_LAMBERT->clone();
        
        else if( instance_of(mesh->getMaterial(), ParticleSystemMaterial))
            return SHADERLIB_PARTICLE_BASIC->clone();
        
        return nullptr;
    }
    
    ptr<ShaderLib> ShaderLib::create( string name, vector<string> precisions, vector<string> defines,
                                     string vertex_params, string vertex_code, string fragment_params, string fragment_code ) {
        return make_shared<ShaderLib>( ShaderLib(name, precisions, defines,
                                                 vertex_params, vertex_code, fragment_params, fragment_code) );
    }
    
    
    ShaderLib::ShaderLib( string name, vector<string> precisions, vector<string> defines, string vertex_params,
                         string vertex_code, string fragment_params, string fragment_code ):
        id            ( "" ),
        name          ( name ),
        config        ( 0 ),
        version       ( SHADER_VERSION ),
        precisions    ( precisions ),
        defines       ( defines ),
        vertexParams  ( vertex_params ),
        vertexCode    ( vertex_code ),
        fragmentParams( fragment_params ),
        fragmentCode  ( fragment_code )
    {}
    
    
    ShaderLib::ShaderLib( const ShaderLib& rhs ) :
        config        ( rhs.config ),
        id            ( rhs.id ),
        name          ( rhs.name ),
        version       ( rhs.version ),
        precisions    ( rhs.precisions ),
        defines       ( rhs.defines ),
        vertexParams  ( rhs.vertexParams ),
        vertexCode    ( rhs.vertexCode ),
        fragmentParams( rhs.fragmentParams ),
        fragmentCode  ( rhs.fragmentCode )
    {}
    
    ShaderLib::~ShaderLib(){}
    
    ptr<ShaderLib> ShaderLib::clone() const {
        return make_shared<ShaderLib>(ShaderLib(*this));
    }
    
    ptr<Shader> ShaderLib::getShader() {
        return shader;
    }
    
    
    /**
     * Create a plethora of #define statements based on the expected configuration of the shader
     */
    void ShaderLib::addDefinitions(ptr<Scene> scene, ptr<Mesh> mesh, bool gamma_input, bool gamma_output) {
        config.reset();
        
        if( scene->getFog() != nullptr ) {
            this->defines.push_back("#define USE_FOG");

            if( instance_of(scene->getFog(), FogExp2))
                this->defines.push_back("#define FOG_EXP2");
        }
    
        if( gamma_input )
            this->defines.push_back("#define GAMMA_INPUT");
        
        if( gamma_output ) {
            this->defines.push_back("#define GAMMA_OUTPUT");
        }
        
        if( mesh->useVertexColors() ) {
            this->defines.push_back("#define USE_COLOR");
            config[8] = 1;
        }
        
        stringstream ss;
        ss.str("");
        ss << "#define MAX_DIR_LIGHTS " << scene->getDirectionalLights().getSize();
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_POINT_LIGHTS " << scene->getPointLights().getSize();
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_HEMI_LIGHTS " << scene->getHemisphereLights().getSize();
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_SPOT_LIGHTS " << scene->getSpotLights().getSize();
        this->defines.push_back( ss.str() );
    
        ss.str("");
        ss << "#define POINT_SIZE " << mesh->getMaterial()->getPointSize();
        this->defines.push_back( ss.str() );
        
        if( mesh->hasTexture() ) {
            this->defines.push_back( "#define USE_MAP" );
            config[0] = 1;
        }
        
        if( mesh->hasNormalMap() ) {
            this->defines.push_back( "#define USE_NORMALMAP" );
            config[1] = 1;
        }
        
        if( mesh->hasSpecularMap() ) {
            this->defines.push_back( "#define USE_SPECULARMAP" );
            config[2] = 1;
        }
        
        if( mesh->hasEnvMap() ) {
            this->defines.push_back( "#define USE_ENVMAP" );
            config[3] = 1;
        }
        
        int shadow_caster_count = scene->getShadowCasterCount();
        if( shadow_caster_count > 0 && mesh->receiveShadow ){
            this->defines.push_back("#define USE_SHADOWMAP");
            config[4] = 1;
            
            ss.str("");
            ss << "#define MAX_SHADOWS " << shadow_caster_count;
            this->defines.push_back( ss.str() );
            
            switch( scene->getShadowMapType() ) {
                case SHADOW_MAP::PCF      : this->defines.push_back( "#define SHADOW_TYPE_PCF" ); break;
                case SHADOW_MAP::PCF_SOFT : this->defines.push_back( "#define SHADOW_TYPE_PCF_SOFT" ); break;
                default: break;
            }
        }
        
        
        if( mesh->getMaterial()->getAlphaTest() > 0.0 ) {
            ss.str("");
            ss << "#define ALPHATEST " << mesh->getMaterial()->getAlphaTest() ;
            this->defines.push_back( ss.str() );
            config[5] = 1;
        }
        
        
        if( mesh->getMaterial()->doesWrapAround() ) {
            this->defines.push_back( "#define WRAP_AROUND" );
            config[6] = 1;
        }
        
        if( mesh->getMaterial()->getSide() == SIDE::DOUBLE_SIDE ) {
            this->defines.push_back( "#define DOUBLE_SIDED" );
            config[7] = 1;
        }
    }
    
    void ShaderLib::compileShader() {
        this->shader = Shader::create( *this );
    }
    
    void ShaderLib::bind() {
        this->shader->bind();
    }
    
    void ShaderLib::unbind() {
        this->shader->unbind();
    }
    
    /**
     * Pass the camera matrices to shader uniforms, and call the draw function on the mesh
     */
    void ShaderLib::draw( ptr<Camera> camera, ptr<CameraControl> cam_control, ptr<Object3D> object, bool gamma_input ) {
        glm::mat4 rot_mat(1.0);
        glm::vec3 cam_position = camera->position * camera->quaternion;
        
        if( cam_control != nullptr) {
            rot_mat = cam_control->createTransformationMatrix();
            cam_position = glm::vec3(glm::vec4(cam_position, 1.0) * rot_mat);
        }
        
        shader->setUniform( "projection_mat",   camera->getProjectionMatrix() );
        shader->setUniform( "view_mat",         camera->matrix * rot_mat );
        shader->setUniform( "camera_pos_w",     cam_position );
        
        if( !object->visible )
            return;
        
        glm::mat4 mv = camera->matrix * rot_mat * object->matrixWorld;
        shader->setUniform( "normal_mat", glm::inverseTranspose( glm::mat3(mv) ) );
        shader->setUniform( "model_view_mat", mv );
        
        if( instance_of( object, ParticleSystem) ) {
            ptr<ParticleSystem> particle_system = downcast(object, ParticleSystem) ;
            particle_system->sort(camera);
            particle_system->setUniforms(shared_from_this(), gamma_input );
            particle_system->draw();
        }
        else if( instance_of(object, Mesh ) ) {
            ptr<Mesh> mesh = downcast(object, Mesh) ;
            mesh->setUniforms(shared_from_this(), gamma_input );
            mesh->draw();
        }
    }

    /**
     * Create unique ID for the shader instance, based
     * on the shader code, configurations and definitions tacked on to it
     */
    string ShaderLib::getID() {
        stringstream ss;
        ss << name << "_";
        ss << config.to_string();
        return ss.str();
    }
    
    /**
     * Check if the fragment or vertex code is empty
     */
    bool ShaderLib::empty() {
        return fragmentCode.empty() || vertexCode.empty();
    }
    
    const bitset<32> ShaderLib::getConfig() {
        return config;
    }
    
    string ShaderLib::getVertexParams() {
        return this->vertexParams;
    }
    
    string ShaderLib::getFragmentParams() {
        return this->fragmentParams;
    }
    
    
    string ShaderLib::getVertexCode() {
        return this->vertexCode;
    }
    
    string ShaderLib::getFragmentCode() {
        return this->fragmentCode;
    }
    
    /**
     * String the fragment shader chunks together into a vertex shader code
     */
    string ShaderLib::constructFragmentShader() {
        return Utils::join({
            this->version,
            Utils::join(this->precisions),
            Utils::join(this->defines),
            this->fragmentParams,
            this->fragmentCode,
        });
    }
    
    /**
     * String the vertex shader chunks together into a vertex shader code
     */
    string ShaderLib::constructVertexShader() {
        return Utils::join({
            this->version,
            Utils::join(this->precisions),
            Utils::join(this->defines),
            this->vertexParams,
            this->vertexCode,
        });
    }
}