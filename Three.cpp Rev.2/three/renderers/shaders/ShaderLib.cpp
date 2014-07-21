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
        if( instance_of(mesh->material, MeshPhongMaterial) ) {
            return ShaderLib_PHONG->clone();
        }
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
        ss << "#define MAX_DIR_LIGHTS " << static_cast<int>(count_if( scene->directionalLights.begin(), scene->directionalLights.end(), [](ptr<DirectionalLight> light){
            return light->visible;
        }));
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_POINT_LIGHTS " << static_cast<int>(count_if( scene->pointLights.begin(), scene->pointLights.end(), [](ptr<PointLight> light){
            return light->visible;
        }));
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_HEMI_LIGHTS " << static_cast<int>(count_if( scene->hemisphereLights.begin(), scene->hemisphereLights.end(), [](ptr<HemisphereLight> light){
            return light->visible;
        }));
        this->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_SPOT_LIGHTS " << static_cast<int>(count_if( scene->spotLights.begin(), scene->spotLights.end(), [](ptr<SpotLight> light){
            return light->visible;
        }));
        this->defines.push_back( ss.str() );
        
        config.reset();
        
        if( mesh->texture != nullptr ) {
            if( instance_of(mesh->texture, EnvMap)) {
                this->defines.push_back( "#define USE_CUBEMAP" );
                config[1] = 1;
            }
            else {
                this->defines.push_back( "#define USE_MAP" );
                config[0] = 1;
            }
        }
        
        
        if( mesh->normalMap != nullptr ) {
            this->defines.push_back( "#define USE_NORMALMAP" );
            config[2] = 1;
        }
        
        
        if( mesh->specularMap!= nullptr ) {
            this->defines.push_back( "#define USE_SPECULARMAP" );
            config[3] = 1;
        }
        
        
        
        if( mesh->envMap != nullptr ) {
            this->defines.push_back( "#define USE_ENVMAP" );
            config[4] = 1;
        }
        
        
        if( mesh->material->alphaTest > 0.0 ) {
            ss.str("");
            ss << "#define ALPHATEST " << mesh->material->alphaTest ;
            this->defines.push_back( ss.str() );
            config[5] = 1;
        }
        
        
        if( mesh->material->wrapAround ) {
            this->defines.push_back( "#define WRAP_AROUND" );
            config[6] = 1;
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
        shader->setUniform( "projection_mat",   camera->projection );
        shader->setUniform( "view_mat",         camera->matrix * arcball->createViewRotationMatrix() );
        shader->setUniform( "camera_pos_w",     camera->position );

        if( !object->visible )
            return;
        
        glm::mat4 mv = camera->matrix * arcball->createViewRotationMatrix() * object->matrixWorld;
        shader->setUniform( "normal_mat", glm::inverseTranspose( glm::mat3(mv) ) );
        shader->setUniform( "model_view_mat", mv );

        if( instance_of(object, Mesh ) ) {
            ptr<Mesh> mesh = downcast(object, Mesh) ;
            mesh->draw(shader, gamma_input);
        }
    }
    
    void ShaderLib::setFog( ptr<IFog> ifog, bool gamma_input ){
        if( ifog == nullptr )
            return;
        
        if( instance_of(ifog, Fog)) {
            ptr<Fog> fog = downcast(ifog, Fog);
            shader->setUniform( "fog_color", fog->color, 1.0, gamma_input );
            shader->setUniform( "fog_near", fog->near );
            shader->setUniform( "fog_far",  fog->far );
        }
        else if( instance_of(ifog, FogExp2)) {
            ptr<FogExp2> fog = downcast(ifog, FogExp2);
            shader->setUniform( "fog_color", fog->color, 1.0, gamma_input );
            shader->setUniform( "fog_density", fog->distance );
        }
    }
    
    void ShaderLib::setAmbientLights( ptr<AmbientLight> light, bool gamma_input ){
        if( light == nullptr )
            shader->setUniform( "ambient_light_color", Color(0x000000), 1.0, gamma_input );
        else {
            if( light->visible )
                shader->setUniform( "ambient_light_color", light->color, 1.0, gamma_input );
        }

    }
    
    void ShaderLib::setHemisphereLights( vector<ptr<HemisphereLight>>& lights, bool gamma_input ){
        size_t size = count_if( lights.begin(), lights.end(),
                               [](ptr<HemisphereLight> light){
                                   return light->visible;
                               });
        if( size == 0 )
            return;
        
        vector<Color> sky_colors;
        vector<Color> ground_colors;
        vector<float> intensities;
        vector<glm::vec3> directions;
        
        for( ptr<HemisphereLight> light: lights ){
            sky_colors.push_back   (light->color);
            ground_colors.push_back(light->groundColor);
            intensities.push_back  (light->intensity);
            directions.push_back   (light->position);
        }
        
        shader->setUniform( "hemisphere_light_sky_color", sky_colors, intensities, gamma_input );
        shader->setUniform( "hemisphere_light_ground_color", ground_colors, intensities, gamma_input );
        shader->setUniform( "hemisphere_light_direction", directions );
    }
    
    void ShaderLib::setDirectionalLights( vector<ptr<DirectionalLight>>& lights, bool gamma_input ){
        size_t size = count_if( lights.begin(), lights.end(),
                       [](ptr<DirectionalLight> light){
                           return light->visible;
                       });
        
        if( size == 0 )
            return;
        
        vector<glm::vec3> directions;
        vector<Color> colors;
        vector<float> intensities;
        
        for( ptr<DirectionalLight> light : lights ) {
            if( !light->visible )
                continue;
            
            directions.push_back ( light->position );
            colors.push_back     ( light->color );
            intensities.push_back( light->intensity );
        }
        
        shader->setUniform( "directional_light_direction", directions );
        shader->setUniform( "directional_light_color", colors, intensities, gamma_input );
    }
    
    void ShaderLib::setPointLights( vector<ptr<PointLight>>& lights, bool gamma_input ){
        size_t size = count_if( lights.begin(), lights.end(),
                               [](ptr<PointLight> light){
                                   return light->visible;
                               });
        if( size == 0 )
            return;
        
        vector<Color> colors;
        vector<glm::vec3> positions;
        vector<float> intensities;
        vector<float> distances;
        
        for( ptr<PointLight> light: lights ) {
            colors.push_back     (light->color);
            intensities.push_back(light->intensity);
            positions.push_back  (light->position);
            distances.push_back  (light->distance);
        }
        
        shader->setUniform( "point_light_color", colors, intensities, gamma_input );
        shader->setUniform( "point_light_position", positions );
        shader->setUniform( "point_light_distance", distances );
    }
    
    void ShaderLib::setSpotLights( vector<ptr<SpotLight>>& lights, bool gamma_input ){
        size_t size = count_if( lights.begin(), lights.end(),
                               [](ptr<SpotLight> light){
                                   return light->visible;
                               });
        
        if( size == 0 )
            return;
        
        vector<Color> colors;
        vector<float> intensities;
        vector<glm::vec3> positions;
        vector<glm::vec3> directions;
        vector<float> cos_angles;
        vector<float> exponents;
        vector<float> distances;
        
        for( ptr<SpotLight> light: lights ){
            colors.push_back     (light->color);
            intensities.push_back(light->intensity);
            positions.push_back  (light->position);
            directions.push_back (glm::normalize(light->position - light->target));
            cos_angles.push_back (cosf(Math::degToRad(light->angle)));
            exponents.push_back  (light->exponent);
            distances.push_back  (light->distance);
        }
        
        
        shader->setUniform( "spot_light_color", colors, intensities, gamma_input );
        shader->setUniform( "spot_light_position", positions );
        shader->setUniform( "spot_light_direction", directions );
        
        shader->setUniform( "spot_light_angle_cos", cos_angles );
        shader->setUniform( "spot_light_exponent", exponents );
        shader->setUniform( "spot_light_distance", distances );
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