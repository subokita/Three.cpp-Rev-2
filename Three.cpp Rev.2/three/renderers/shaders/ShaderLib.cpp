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

using namespace std;

namespace three {
    ptr<ShaderLib> ShaderLib::create( ptr<Mesh> mesh ) {
        if( instance_of(mesh->material, PhongMaterial) )
            return SHADERLIB_PHONG->clone();
        
        else if ( instance_of(mesh->material, MeshCubeMapMaterial ) )
            return SHADERLIB_CUBEMAP->clone();
        
        else if ( instance_of(mesh->material, BasicMaterial ) )
            return SHADERLIB_BASIC->clone();
        
        else if ( instance_of(mesh->material, LambertMaterial))
            return SHADERLIB_LAMBERT->clone();
        
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
    {
    }
    
    
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
    
    
    ptr<ShaderLib> ShaderLib::clone() const {
        return make_shared<ShaderLib>(ShaderLib(*this));
    }
    
    ptr<Shader> ShaderLib::getShader() {
        return shader;
    }
    
    void ShaderLib::addDefinitions(ptr<Scene> scene, ptr<Mesh> mesh, bool gamma_input, bool gamma_output) {
        if( scene->fog != nullptr ) {
            this->defines.push_back("#define USE_FOG");
            
            if( instance_of(scene->fog, FogExp2))
                this->defines.push_back("#define FOG_EXP2");
        }
        
        if( gamma_input )
            this->defines.push_back("#define GAMMA_INPUT");
        
        if( gamma_output )
            this->defines.push_back("#define GAMMA_OUTPUT");
        
        stringstream ss;
        ss.str("");
        ss << "#define MAX_DIR_LIGHTS " << scene->directionalLights.getSize();
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_POINT_LIGHTS " << scene->pointLights.getSize();
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_HEMI_LIGHTS " << scene->hemisphereLights.getSize();
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_SPOT_LIGHTS " << scene->spotLights.getSize();
        this->defines.push_back( ss.str() );
        
        config.reset();
        
        if( mesh->texture != nullptr ) {
            this->defines.push_back( "#define USE_MAP" );
            config[0] = 1;
        }
        
        if( mesh->normalMap != nullptr ) {
            this->defines.push_back( "#define USE_NORMALMAP" );
            config[1] = 1;
        }
        
        if( mesh->specularMap!= nullptr ) {
            this->defines.push_back( "#define USE_SPECULARMAP" );
            config[2] = 1;
        }
        
        if( mesh->envMap != nullptr ) {
            this->defines.push_back( "#define USE_ENVMAP" );
            config[3] = 1;
        }
        
        
        if( mesh->material->alphaTest > 0.0 ) {
            ss.str("");
            ss << "#define ALPHATEST " << mesh->material->alphaTest ;
            this->defines.push_back( ss.str() );
            config[4] = 1;
        }
        
        
        if( mesh->material->wrapAround ) {
            this->defines.push_back( "#define WRAP_AROUND" );
            config[5] = 1;
        }
        
        if( mesh->material->side == SIDE::DOUBLE_SIDE ) {
            this->defines.push_back( "#define DOUBLE_SIDED" );
            config[6] = 1;
        }
        
        if( scene->getShadowCasterCount() > 0 ){
            this->defines.push_back("#define USE_SHADOWMAP");
            config[7] = 1;
            
            this->defines.push_back( "#define SHADOW_TYPE_PCF_SOFT" );
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
     * 
     */
    void ShaderLib::draw( ptr<Camera> camera, ptr<Arcball> arcball, ptr<Object3D> object, bool gamma_input ) {
        glm::mat4 rot_mat(1.0);
        if( arcball != nullptr) {
            rot_mat = arcball->createViewRotationMatrix();
        }
        
        shader->setUniform( "projection_mat",   camera->projection );
        shader->setUniform( "view_mat",         camera->matrix * rot_mat );
        shader->setUniform( "camera_pos_w",     camera->position );
        
        if( !object->visible )
            return;
        
        glm::mat4 mv = camera->matrix * rot_mat * object->matrixWorld;
        shader->setUniform( "normal_mat", glm::inverseTranspose( glm::mat3(mv) ) );
        shader->setUniform( "model_view_mat", mv );
        
        if( instance_of(object, Mesh ) ) {
            ptr<Mesh> mesh = downcast(object, Mesh) ;
            mesh->setUniforms(shader, gamma_input );
            mesh->draw();
        }
    }

 
    string ShaderLib::getID() {
        stringstream ss;
        ss << name << "_";
        ss << hex << config.to_ullong();
        return ss.str();
    }
    
    bool ShaderLib::empty() {
        return fragmentCode.empty() || vertexCode.empty();
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
    
    string ShaderLib::constructFragmentShader() {
        return Utils::join({
            this->version,
            Utils::join(this->precisions),
            Utils::join(this->defines),
            this->fragmentParams,
            this->fragmentCode,
        });
    }
    
    string ShaderLib::constructVertexShader() {
        return Utils::join({
            this->version,
            Utils::join(this->precisions),
            Utils::join(this->defines),
            this->vertexParams,
            this->vertexCode,
        });
    }
    
    ShaderLib::~ShaderLib(){}
}