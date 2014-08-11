
layout (location = 0) in vec3 vertex_pos_m;
layout (location = 1) in vec3 vertex_normal_m;
layout (location = 2) in vec2 vertex_uv_m;
uniform mat4 model_mat;
uniform mat4 model_view_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat3 normal_mat;
uniform vec3 camera_pos_w;

out vec3 normal_c;
out vec3 eye_direction_c;
out vec3 vertex_pos_w;

#if defined( USE_MAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )
    out vec2 uv;
    uniform vec4 offset_repeat;
#endif
#ifdef USE_MAP
    uniform sampler2D map;
#endif

#if defined( USE_ENVMAP ) && !defined( USE_BUMPMAP ) && !defined( USE_NORMALMAP )
    out vec3 reflect_c;
    uniform float refraction_ratio;
    uniform bool use_refraction;
#endif

#ifdef USE_SHADOWMAP
    out vec4 shadow_coord_c[MAX_SHADOWS];
    uniform mat4 shadow_mat[MAX_SHADOWS];
#endif

void main() {
    
#if defined( USE_MAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )
	uv = vertex_uv_m * offset_repeat.zw + offset_repeat.xy;
#endif
    
    eye_direction_c   = vec3(0.0) - ( model_view_mat * vec4(vertex_pos_m, 1.0) ).xyz;
	normal_c          = normalize(normal_mat * vec3(vertex_normal_m));
	vertex_pos_w      = vec3( model_mat * vec4(vertex_pos_m, 1.0) );
	gl_Position       = projection_mat * model_view_mat * vec4(vertex_pos_m, 1.0);
	
#if defined( USE_ENVMAP ) && !defined( USE_BUMPMAP ) && !defined( USE_NORMALMAP )
	vec3 vertex_normal_w = normalize((model_mat * vec4(vertex_normal_m, 0.0)).xyz);
	vec3 camera_to_vert  = normalize( (vertex_pos_w - camera_pos_w) );
	if( use_refraction ) {
		reflect_c = refract(camera_to_vert, vertex_normal_w, refraction_ratio );
	}
	else {
		reflect_c = reflect(camera_to_vert, vertex_normal_w );
	}
#endif
    
#ifdef USE_SHADOWMAP
	for( int i = 0; i < MAX_SHADOWS; i++ ) {
		shadow_coord_c[i] = shadow_mat[i] * vec4(vertex_pos_w, 1.0);
	}
#endif
}
