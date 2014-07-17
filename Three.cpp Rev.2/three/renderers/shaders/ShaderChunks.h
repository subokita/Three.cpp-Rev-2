//
//  Utils.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Utils__
#define __Three_cpp_Rev_2__Utils__

#include <iostream>
#include <vector>

#include "Utils.h"

namespace three {
    using namespace std;
    
    namespace Chunks {
        static const string standardVertexParams = Utils::join({
            "layout (location = 0) in vec3 vertex_pos_m;",
            "layout (location = 1) in vec3 vertex_normal_m;",
            "layout (location = 2) in vec2 vertex_uv_m;",
            
            "uniform mat4 model_mat;",
            "uniform mat4 model_view_mat;",
            "uniform mat4 view_mat;",
            "uniform mat4 projection_mat;",
            "uniform mat3 normal_mat;",
            "uniform vec3 camera_pos_w;",
        });
        
        static const string standardFragmentParams = Utils::join({
            "out vec4 frag_color;",
        });
        
        static const string textureVertexParams = Utils::join({
            "#ifdef USE_MAP",
                "out vec2 uv;",
                "uniform sampler2D map;",
            "#endif",
        });
        
        static const string textureFragmentParams = Utils::join({
            "#ifdef USE_MAP",
                "in vec2 uv;",
                "uniform sampler2D map;",
            "#endif",
        });
        
        
        static const string textureVertex = Utils::join({
            "#ifdef USE_MAP",
                "uv = vertex_uv_m;",
            "#endif",
        });
        
        static const string textureFragment = Utils::join({
            "#ifdef USE_MAP",
                "vec4 texel_color = texture(map, uv);",
            
                "#ifdef GAMMA_INPUT",
                    "texel_color.xyz *= texel_color.xyz;",
                "#endif",
            
                "frag_color = frag_color * texel_color;",
            "#endif",
        });
        
        static const string phongVertexParams = Utils::join({
            "out vec3 normal_c;",
            "out vec3 eye_direction_c;",
            "out vec3 vertex_pos_w;",
        });
        
        static const string phongVertex = Utils::join({
            "eye_direction_c   = vec3(0.0) - ( model_view_mat * vec4(vertex_pos_m, 1.0) ).xyz;",
            "normal_c          = normalize(normal_mat * vec3(vertex_normal_m));",
            "vertex_pos_w      = vec3( model_mat * vec4(vertex_pos_m, 1.0) );",
            "gl_Position       = projection_mat * model_view_mat * vec4(vertex_pos_m, 1.0);",
        }, "\t" );
        
        static const string phongFragmentParams = Utils::join({
            "uniform vec3 ambient_light_color;",
            
            "uniform mat4 view_mat;",
            
            "uniform float opacity;",
            "uniform vec3 emissive;",
            "uniform vec3 ambient;",
            "uniform vec3 diffuse;",
            "uniform vec3 specular;",
            "uniform float shininess;",
            
            "in vec3 normal_c;",
            "in vec3 eye_direction_c;",
            "in vec3 vertex_pos_w;",
            
            "#ifdef WRAP_AROUND",
                "uniform vec3 wrapRGB;",
            "#endif",
        });
        
        
        static const string phongFragment_1 = Utils::join({
            "frag_color = vec4( vec3( 1.0 ), opacity );",
            
            "vec3 total_diffuse        = vec3(0.0);",
            "vec3 total_specular       = vec3(0.0);",
            "vec3 norm_normal_c        = normalize( normal_c );",
            "vec3 norm_eye_direction_c = normalize( eye_direction_c );",
            "float specular_strength   = 1.0;",
            "float specular_norm       = (shininess + 2.0001) / 8.0;",
        }, "\t");
        
        static const string phongFragment_2 = Utils::join({
            "#ifdef METAL",
                "frag_color.xyz = frag_color.xyz * ( emissive + ambient_light_color * ambient + total_diffuse + total_specular );",
            "#else",
                "frag_color.xyz = frag_color.xyz * ( emissive + ambient_light_color * ambient + total_diffuse ) + total_specular;",
            "#endif",
            "#ifdef GAMMA_OUTPUT",
                "frag_color.xyz = sqrt(frag_color.xyz);",
            "#endif",
        }, "\t" );
        
        
        static const string pointLightsFragmentParams = Utils::join({
            "#if MAX_POINT_LIGHTS > 0",
                "uniform vec3 point_light_color     [MAX_POINT_LIGHTS];",
                "uniform vec3 point_light_position  [MAX_POINT_LIGHTS];",
                "uniform float point_light_distance [MAX_POINT_LIGHTS];",
            "#endif",
        });
        
        
        static const string pointLightsFragment = Utils::join({
            "#if MAX_POINT_LIGHTS > 0",
            
                "vec3 point_diffuse  = vec3( 0.0 );",
                "vec3 point_specular = vec3( 0.0 );",
                
                "for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {",
                    "vec4 light_position_c = view_mat * vec4( point_light_position[i], 1.0 );",
                    "vec3 l = light_position_c.xyz + eye_direction_c.xyz;",
                    "float light_distance = 1.0;",
                    "if( point_light_distance[i] > 0.0 )",
                    "   light_distance = 1.0 - min( (length(l) / point_light_distance[i]), 1.0 );",
            
                    /* Calculate diffuse component */
                    "l = normalize( l );",
                    "float cos_theta = dot( norm_normal_c, l );",
            
                    "#ifdef WRAP_AROUND",
                        "float diff_weight_full = max( cos_theta, 0.0 );",
                        "float diff_weight_half = max( 0.5 * cos_theta + 0.5, 0.0 );",
                        "vec3 diff_weight       = mix( vec3(diff_weight_full), vec3(diff_weight_half), wrapRGB );",
                    "#else",
                        "float diff_weight      = max( cos_theta, 0.0 );",
                    "#endif",
            
                    "point_diffuse += diffuse * point_light_color[i] * diff_weight * light_distance;",
            
                    /* Calculate specular component */
                    "vec3 r             = normalize( l + norm_eye_direction_c );",
                    "float cos_alpha    = max( dot( norm_normal_c, r ), 0.0 );",
                    "float spec_weight  = specular_strength * max( pow(cos_alpha, shininess), 0.0);",
                    "vec3 schlick   = specular + vec3(1.0 - specular) * pow( max( 1.0 - dot( l, r ), 0.0) , 5.0);",
                    "point_specular += schlick * point_light_color[i] * spec_weight * diff_weight * light_distance * specular_norm;",
                "}",
                
                "total_diffuse  += point_diffuse;",
                "total_specular += point_specular;",
            "#endif",
        }, "\t");
        
        static const string directionalLightsFragmentParams = Utils::join({
            "#if MAX_DIR_LIGHTS > 0",
                "uniform vec3 directional_light_color    [MAX_DIR_LIGHTS];",
                "uniform vec3 directional_light_direction[MAX_DIR_LIGHTS];",
            "#endif",
        });
        
        static const string directionalLightsFragment = Utils::join({
            "#if MAX_DIR_LIGHTS > 0",
            
            "vec3 dir_diffuse  = vec3( 0.0 );",
            "vec3 dir_specular = vec3( 0.0 );",
            
            "for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {",
                "vec4 light_direction_c = view_mat * vec4( directional_light_direction[ i ], 0.0 );",
            
                /* Calculating the diffuse component */
                "vec3 l = normalize( light_direction_c.xyz );",
                "float cos_theta = dot( norm_normal_c, l );",
            
                "#ifdef WRAP_AROUND",
                    "float diff_weight_full = max( cos_theta, 0.0 );",
                    "float diff_weight_half = max( 0.5 * cos_theta + 0.5, 0.0 );",
                    "vec3 diff_weight       = mix( vec3(diff_weight_full), vec3(diff_weight_half), wrapRGB );",
                "#else",
                    "float diff_weight = max( cos_theta, 0.0 );",
                "#endif",
            
                "dir_diffuse += diffuse * directional_light_color[i] * diff_weight;",
            
                /* Calculating specular component */
                "vec3 r             = normalize( l + norm_eye_direction_c );",
                "float cos_alpha    = max( dot(norm_normal_c, r), 0.0);",
            
                "float spec_weight        = specular_strength * max( pow( cos_alpha, shininess ), 0.0 );",
                "vec3 schlick = specular + vec3(1.0 - specular) * pow( max(1.0 - dot(l, r), 0.0) , 5.0);",
                "dir_specular += schlick * directional_light_color[i] * spec_weight * diff_weight * specular_norm;",
            "}",
            
            "total_diffuse  += dir_diffuse;",
            "total_specular += dir_specular;",
            "#endif",
        }, "\t");
        
        
        static const string hemisphereLightsFragmentParams = Utils::join({
            "#if MAX_HEMI_LIGHTS > 0",
                "uniform vec3 hemisphere_light_sky_color    [MAX_HEMI_LIGHTS];",
                "uniform vec3 hemisphere_light_ground_color [MAX_HEMI_LIGHTS];",
                "uniform vec3 hemisphere_light_direction    [MAX_HEMI_LIGHTS];",
            "#endif",
        });
        
        
        static const string hemisphereLightsFragment = Utils::join({
            "#if MAX_HEMI_LIGHTS > 0",
                
                "vec3 hemi_diffuse  = vec3( 0.0 );",
                "vec3 hemi_specular = vec3( 0.0 );",
                
                "for( int i = 0; i < MAX_HEMI_LIGHTS; i ++ ) {",
                    "vec4 light_direction_c = view_mat * vec4( hemisphere_light_direction[i], 0.0 );",
                    "vec3 l = normalize( light_direction_c.xyz );",
                    "float cos_theta = dot( norm_normal_c, l);",

                    "float diff_weight = 0.5 * cos_theta + 0.5;",
                    "vec3 hemi_color = mix( hemisphere_light_ground_color[i], hemisphere_light_sky_color[i], diff_weight );",
                    "hemi_diffuse += diffuse * hemi_color;",
            
                    "vec3 r_sky = normalize( l + norm_eye_direction_c );",
                    "float cos_alpha_sky = 0.5 * dot( norm_normal_c, r_sky ) + 0.5;",
                    "float spec_weight_sky = specular_strength * max( pow(cos_alpha_sky, shininess), 0.0);",
            
                    "vec3 l_ground = -l;",
                    "vec3 r_ground = normalize( l_ground + norm_eye_direction_c );",
                    "float cos_alpha_ground = 0.5 * dot( norm_normal_c, r_ground ) + 0.5;",
                    "float spec_weight_ground = specular_strength * max( pow(cos_alpha_ground, shininess), 0.0);",
                    "float cos_theta_ground = dot( norm_normal_c, l_ground );",
            
                    "vec3 schlick_sky    = specular + vec3( 1.0 - specular) * pow( max( 1.0 - dot(l, r_sky), 0.0) , 5.0);",
                    "vec3 schlick_ground = specular + vec3( 1.0 - specular) * pow( max( 1.0 - dot(l_ground, r_ground), 0.0) , 5.0);",
                    "hemi_specular      += hemi_color * specular_norm * (schlick_sky * spec_weight_sky * max( cos_theta, 0.0 ) + schlick_ground * spec_weight_ground * max(cos_theta_ground, 0.0) );",
                "}",
                
                "total_diffuse  += hemi_diffuse;",
                "total_specular += hemi_specular;",
            "#endif",
        }, "\t");
        
        
        static const string spotLightsFragmentParams = Utils::join({
            "#if MAX_SPOT_LIGHTS > 0",
                "uniform vec3 spot_light_color     [MAX_SPOT_LIGHTS];",
                "uniform vec3 spot_light_position  [MAX_SPOT_LIGHTS];",
                "uniform vec3 spot_light_direction [MAX_SPOT_LIGHTS];",
                "uniform float spot_light_angle_cos[MAX_SPOT_LIGHTS];",
                "uniform float spot_light_exponent [MAX_SPOT_LIGHTS];",
                "uniform float spot_light_distance [MAX_SPOT_LIGHTS];",
            "#endif",
        });
        
        
        static const string spotLightsFragment = Utils::join({
            "#if MAX_SPOT_LIGHTS > 0",
                "vec3 spot_diffuse  = vec3( 0.0 );",
                "vec3 spot_specular = vec3( 0.0 );",
                
                "for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {",
                    "vec4 light_position = view_mat * vec4( spot_light_position[i], 1.0);",
                    "vec3 l = light_position.xyz + norm_eye_direction_c.xyz;",
            
                    "float light_distance = 1.0;",
                    "if( spot_light_distance[i] > 0.0 )",
                    "    light_distance = 1.0 - min( length(l) / spot_light_distance[i], 1.0);",
                    "l = normalize( l ) ;",
            
                    "float spot_effect = dot( spot_light_direction[i], normalize( spot_light_position[i] - vertex_pos_w ) );",
                    "if( spot_effect > spot_light_angle_cos[i] ) {",
                        "spot_effect = max( pow( spot_effect, spot_light_exponent[i] ), 0.0 );",
                        "float cos_theta = dot( norm_normal_c, l );",
            
                        "#ifdef WRAP_AROUND",
                            "float diff_weight_full = max( cos_theta, 0.0 );",
                            "float diff_weight_half = max( 0.5 * cos_theta + 0.5, 0.0 );",
                            "float diff_weight = mix( vec3(diff_weight_full), vec3(, diff_weight_half), wrapRGB );",
                        "#else",
                            "float diff_weight = max( cos_theta, 0.0 );",
                        "#endif",
            
                        "spot_diffuse += diffuse * spot_light_color[i] * diff_weight * light_distance * spot_effect;",
            
                        "vec3 r = normalize( l + norm_eye_direction_c );",
                        "float cos_alpha = max( dot( norm_normal_c, r ), 0.0 );",
                        "float spec_weight = specular_strength * max( pow(cos_alpha, shininess), 0.0);",
                        "vec3 schlick = specular + vec3(1.0 - specular) * pow( max( 1.0 - dot( l, r), 0.0), 5.0 );",
                        "spot_specular += schlick * spot_light_color[i] * spec_weight * diff_weight * light_distance * specular_norm;",
                    "}",

                "}",
                "total_diffuse  += spot_diffuse;",
                "total_specular += spot_specular;",
            
            "#endif",
        }, "\t");
        
        
        static const string fogFragmentParams = Utils::join({
            "#ifdef USE_FOG",
                "uniform vec3 fog_color;",
            
                "#ifdef FOG_EXP2",
                    "uniform float fog_density;",
                "#else",
                    "uniform float fog_near;",
                    "uniform float fog_far;",
                "#endif",
            "#endif",
        });
        
        static const string fogFragment = Utils::join({
            "#ifdef USE_FOG",
                "#ifdef USE_LOGDEPTHBUF_EXT",
                    "float depth = gl_FragDepthEXT / gl_FragCoord.w;",
                "#else",
                    "float depth = (gl_FragCoord.z / gl_FragCoord.w);",
                "#endif",
                
                "#ifdef FOG_EXP2",
                    "const float LOG2 = 1.442695;",
                    "float fog_factor = exp2( -fog_density * fog_density * depth * depth * LOG2 );",
                    "fog_factor = 1.0 - clamp( fog_factor, 0.0, 1.0 );",
                "#else",
                    "float fog_factor = smoothstep( fog_near, fog_far, depth );",
                "#endif",
            
                "frag_color = mix( frag_color, vec4( fog_color, frag_color.w ), fog_factor );",
            
            "#endif",
        }, "\t" );
    }
}

#endif /* defined(__Three_cpp_Rev_2__Utils__) */
