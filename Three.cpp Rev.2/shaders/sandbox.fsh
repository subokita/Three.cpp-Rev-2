
#version 410 core
#define USE_FOG
#define FOG_EXP2
#define MAX_DIR_LIGHTS 1

uniform mat4 view_mat;

out vec4 frag_color;
uniform vec3 ambient_light_color;
uniform float opacity;
uniform vec3 emissive;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

#if MAX_DIR_LIGHTS > 0
    uniform vec3 directional_light_color[ MAX_DIR_LIGHTS ];
    uniform vec3 directional_light_direction[ MAX_DIR_LIGHTS ];
#endif

in vec3 normal_c;
in vec3 eye_direction_c;

#ifdef USE_FOG
    uniform vec3 fog_color;
    #ifdef FOG_EXP2
        uniform float fog_density;
    #else
        uniform float fog_near;
        uniform float fog_far;
    #endif
#endif

void main() {
	frag_color = vec4( vec3( 1.0 ), opacity );
    
    vec3 total_diffuse  = vec3(0.0);
    vec3 total_specular = vec3(0.0);
    
    #if MAX_DIR_LIGHTS > 0
        vec3 dir_diffuse  = vec3( 0.0 );
        vec3 dir_specular = vec3( 0.0 );
        vec3 n = normalize( normal_c );
        
        for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {
            vec4 light_direction_c = view_mat * vec4( directional_light_direction[ i ], 0.0 );
            
            vec3 l = normalize( light_direction_c.xyz );
            float cos_theta = max( dot( n, l ), 0.0 );
            dir_diffuse  += diffuse * directional_light_color[ i ] * cos_theta;
            
            vec3 e          = normalize( eye_direction_c );
            vec3 r          = reflect( -l, n );
            float cos_alpha = clamp( dot(e, r), 0, 1 );
            dir_specular += specular * directional_light_color[ i ] * pow(cos_alpha, shininess);
        }
        
        total_diffuse  += dir_diffuse;
        total_specular += dir_specular;
    #endif
    
	frag_color.xyz = ( emissive + ambient_light_color * ambient + total_diffuse + total_specular );
    
    #ifdef USE_FOG
        float depth = gl_FragCoord.z / gl_FragCoord.w;
        #ifdef FOG_EXP2
            const float LOG2 = 1.442695;
            float fog_factor = exp2( -fog_density * fog_density * depth * depth * LOG2 );
            fog_factor = 1.0 - clamp( fog_factor, 0.0, 1.0 );
        #else
            float fog_factor = smoothstep( fog_near, fog_far, depth );
        #endif
        frag_color  = mix( frag_color, vec4( fog_color, frag_color.w ), fog_factor );
    #endif
}