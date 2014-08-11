out vec4 frag_color;
uniform vec3 camera_pos_w;

uniform vec3 ambient_light_color;
uniform mat4 view_mat;
uniform float opacity;
uniform vec3 emissive;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

in vec3 normal_c;
in vec3 eye_direction_c;
in vec3 vertex_pos_w;

#ifdef WRAP_AROUND
    uniform vec3 wrapRGB;
#endif

#if MAX_DIR_LIGHTS > 0
    uniform vec3 directional_light_color    [MAX_DIR_LIGHTS];
    uniform vec3 directional_light_direction[MAX_DIR_LIGHTS];
#endif

#if MAX_POINT_LIGHTS > 0
    uniform vec3 point_light_color     [MAX_POINT_LIGHTS];
    uniform vec3 point_light_position  [MAX_POINT_LIGHTS];
    uniform float point_light_distance [MAX_POINT_LIGHTS];
#endif

#if MAX_HEMI_LIGHTS > 0
    uniform vec3 hemisphere_light_sky_color    [MAX_HEMI_LIGHTS];
    uniform vec3 hemisphere_light_ground_color [MAX_HEMI_LIGHTS];
    uniform vec3 hemisphere_light_direction    [MAX_HEMI_LIGHTS];
#endif

#if MAX_SPOT_LIGHTS > 0
    uniform vec3 spot_light_color     [MAX_SPOT_LIGHTS];
    uniform vec3 spot_light_position  [MAX_SPOT_LIGHTS];
    uniform vec3 spot_light_direction [MAX_SPOT_LIGHTS];
    uniform float spot_light_angle_cos[MAX_SPOT_LIGHTS];
    uniform float spot_light_exponent [MAX_SPOT_LIGHTS];
    uniform float spot_light_distance [MAX_SPOT_LIGHTS];
#endif

#ifdef USE_FOG
    uniform vec3 fog_color;
#ifdef FOG_EXP2
    uniform float fog_density;
#else
    uniform float fog_near;
    uniform float fog_far;
#endif
#endif

#if defined( USE_MAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )
    in vec2 uv;
#endif

#ifdef USE_MAP
    uniform sampler2D map;
#endif

#ifdef USE_NORMALMAP
    uniform sampler2D normal_map;
    uniform vec2 normal_scale;

    vec3 perturbNormal2Arb( vec3 eye_pos, vec3 surf_norm ) {
        vec3 q0    = dFdx( eye_pos.xyz );
        vec3 q1    = dFdy( eye_pos.xyz );
        vec2 st0   = dFdx( uv.st );
        vec2 st1   = dFdy( uv.st );
        vec3 S     = normalize(  q0 * st1.t - q1 * st0.t );
        vec3 T     = normalize( -q0 * st1.s + q1 * st0.s );
        vec3 N     = normalize( surf_norm );
        vec3 mapN  = texture( normal_map, uv ).xyz * 2.0 - 1.0;
        mapN.xy    = normal_scale * mapN.xy;
        mat3 tsn   = mat3( S, T, N );
        return normalize( tsn * mapN );
    }
#endif

#ifdef USE_SPECULARMAP
    uniform sampler2D specular_map;
#endif

#ifdef USE_ENVMAP
    uniform float reflectivity;
    uniform int combine;
    uniform float flip_env_map;
    uniform samplerCube env_map;

    #if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )
        uniform bool use_refraction;
        uniform float refraction_ratio;
    #else
        in vec3 reflect_c;
    #endif
#endif

#ifdef USE_SHADOWMAP
    in vec4 shadow_coord_c         [MAX_SHADOWS];
    uniform float shadow_bias      [MAX_SHADOWS];
    uniform float shadow_darkness  [MAX_SHADOWS];
    uniform vec2 shadow_map_size   [MAX_SHADOWS];
    uniform sampler2D shadow_map   [MAX_SHADOWS];

    float unpackDepth( const in vec4 rgba_depth ) {
        const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ), 1.0 / ( 256.0 * 256.0 ), 1.0 / 256.0, 1.0 );
        float depth = dot( rgba_depth, bit_shift );
        return depth;
    }
#endif

void main() {
    frag_color = vec4( vec3( 1.0 ), opacity );
	vec3 total_diffuse        = vec3(0.0);
	vec3 total_specular       = vec3(0.0);
	vec3 norm_normal_c        = normalize( normal_c );
	vec3 norm_eye_direction_c = normalize( eye_direction_c );
	float specular_strength   = 1.0;
	float specular_norm       = (shininess + 2.0001) / 8.0;
	
#ifdef USE_MAP
	vec4 texel_color = texture(map, uv);
#ifdef GAMMA_INPUT
    texel_color.xyz *= texel_color.xyz;
#endif
	frag_color = frag_color * texel_color;
#endif
    
#ifdef ALPHATEST
	if( frag_color.a < ALPHATEST ) discard;
#endif
    
#ifdef USE_SPECULARMAP
    vec4 texel_specular = texture( specular_map, uv );
    specular_strength   = texel_specular.r;
#endif
	
#ifdef USE_NORMALMAP
    norm_normal_c = perturbNormal2Arb( -eye_direction_c, norm_normal_c );
#endif
	
#if MAX_POINT_LIGHTS > 0
    vec3 point_diffuse  = vec3( 0.0 );
    vec3 point_specular = vec3( 0.0 );
    for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {
		vec4 light_position_c = view_mat * vec4( point_light_position[i], 1.0 );
		vec3 l = light_position_c.xyz + eye_direction_c.xyz;
		float light_distance = 1.0;
		if( point_light_distance[i] > 0.0 )
            light_distance = 1.0 - min( (length(l) / point_light_distance[i]), 1.0 );
		l = normalize( l );
		float cos_theta = dot( norm_normal_c, l );
        
#ifdef WRAP_AROUND
        float diff_weight_full = max( cos_theta, 0.0 );
        float diff_weight_half = max( 0.5 * cos_theta + 0.5, 0.0 );
        vec3 diff_weight       = mix( vec3(diff_weight_full), vec3(diff_weight_half), wrapRGB );
#else
        float diff_weight      = max( cos_theta, 0.0 );
#endif
        
		point_diffuse += diffuse * point_light_color[i] * diff_weight * light_distance;
		vec3 r             = normalize( l + norm_eye_direction_c );
		float cos_alpha    = max( dot( norm_normal_c, r ), 0.0 );
		float spec_weight  = specular_strength * max( pow(cos_alpha, shininess), 0.0);
		vec3 schlick   = specular + vec3(1.0 - specular) * pow( max( 1.0 - dot( l, r ), 0.0) , 5.0);
		point_specular += schlick * point_light_color[i] * spec_weight * diff_weight * light_distance * specular_norm;
    }
    total_diffuse  += point_diffuse;
    total_specular += point_specular;
#endif
	
#if MAX_DIR_LIGHTS > 0
    vec3 dir_diffuse  = vec3( 0.0 );
    vec3 dir_specular = vec3( 0.0 );
    
    for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {
		vec4 light_direction_c = view_mat * vec4( directional_light_direction[ i ], 0.0 );
		vec3 l = normalize( light_direction_c.xyz );
		float cos_theta = dot( norm_normal_c, l );
#ifdef WRAP_AROUND
        float diff_weight_full = max( cos_theta, 0.0 );
        float diff_weight_half = max( 0.5 * cos_theta + 0.5, 0.0 );
        vec3 diff_weight       = mix( vec3(diff_weight_full), vec3(diff_weight_half), wrapRGB );
#else
        float diff_weight = max( cos_theta, 0.0 );
#endif
		dir_diffuse += diffuse * directional_light_color[i] * diff_weight;
		vec3 r             = normalize( l + norm_eye_direction_c );
		float cos_alpha    = max( dot(norm_normal_c, r), 0.0);
		float spec_weight        = specular_strength * max( pow( cos_alpha, shininess ), 0.0 );
		vec3 schlick = specular + vec3(1.0 - specular) * pow( max(1.0 - dot(l, r), 0.0) , 5.0);
		dir_specular += schlick * directional_light_color[i] * spec_weight * diff_weight * specular_norm;
    }
    total_diffuse  += dir_diffuse;
    total_specular += dir_specular;
#endif
	
#if MAX_HEMI_LIGHTS > 0
    vec3 hemi_diffuse  = vec3( 0.0 );
    vec3 hemi_specular = vec3( 0.0 );
    for( int i = 0; i < MAX_HEMI_LIGHTS; i ++ ) {
		vec4 light_direction_c = view_mat * vec4( hemisphere_light_direction[i], 0.0 );
		vec3 l = normalize( light_direction_c.xyz );
		float cos_theta = dot( norm_normal_c, l);
		float diff_weight = 0.5 * cos_theta + 0.5;
		vec3 hemi_color = mix( hemisphere_light_ground_color[i], hemisphere_light_sky_color[i], diff_weight );
		hemi_diffuse += diffuse * hemi_color;
        
		vec3 r_sky = normalize( l + norm_eye_direction_c );
		float cos_alpha_sky = 0.5 * dot( norm_normal_c, r_sky ) + 0.5;
		float spec_weight_sky = specular_strength * max( pow(cos_alpha_sky, shininess), 0.0);
		vec3 l_ground = -l;
		vec3 r_ground = normalize( l_ground + norm_eye_direction_c );
		float cos_alpha_ground = 0.5 * dot( norm_normal_c, r_ground ) + 0.5;
		float spec_weight_ground = specular_strength * max( pow(cos_alpha_ground, shininess), 0.0);
		float cos_theta_ground = dot( norm_normal_c, l_ground );
		vec3 schlick_sky    = specular + vec3( 1.0 - specular) * pow( max( 1.0 - dot(l, r_sky), 0.0) , 5.0);
		vec3 schlick_ground = specular + vec3( 1.0 - specular) * pow( max( 1.0 - dot(l_ground, r_ground), 0.0) , 5.0);
		hemi_specular      += hemi_color * specular_norm * (schlick_sky * spec_weight_sky * max( cos_theta, 0.0 ) + schlick_ground * spec_weight_ground * max(cos_theta_ground, 0.0) );
    }
    total_diffuse  += hemi_diffuse;
    total_specular += hemi_specular;
#endif
	
#if MAX_SPOT_LIGHTS > 0
    vec3 spot_diffuse  = vec3( 0.0 );
    vec3 spot_specular = vec3( 0.0 );
    for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {
		vec4 light_position = view_mat * vec4( spot_light_position[i], 1.0);
		vec3 l = light_position.xyz + norm_eye_direction_c.xyz;
		float light_distance = 1.0;
		if( spot_light_distance[i] > 0.0 )
		    light_distance = 1.0 - min( length(l) / spot_light_distance[i], 1.0);
		l = normalize( l ) ;
		float spot_effect = dot( spot_light_direction[i], normalize( spot_light_position[i] - vertex_pos_w ) );
		if( spot_effect > spot_light_angle_cos[i] ) {
            spot_effect = max( pow( spot_effect, spot_light_exponent[i] ), 0.0 );
            float cos_theta = dot( norm_normal_c, l );
#ifdef WRAP_AROUND
			float diff_weight_full = max( cos_theta, 0.0 );
			float diff_weight_half = max( 0.5 * cos_theta + 0.5, 0.0 );
			float diff_weight = mix( vec3(diff_weight_full), vec3(, diff_weight_half), wrapRGB );
#else
			float diff_weight = max( cos_theta, 0.0 );
#endif
            spot_diffuse += diffuse * spot_light_color[i] * diff_weight * light_distance * spot_effect;
            vec3 r = normalize( l + norm_eye_direction_c );
            float cos_alpha = max( dot( norm_normal_c, r ), 0.0 );
            float spec_weight = specular_strength * max( pow(cos_alpha, shininess), 0.0);
            vec3 schlick = specular + vec3(1.0 - specular) * pow( max( 1.0 - dot( l, r), 0.0), 5.0 );
            spot_specular += schlick * spot_light_color[i] * spec_weight * diff_weight * light_distance * specular_norm;
		}
    }
    total_diffuse  += spot_diffuse;
    total_specular += spot_specular;
#endif
	
#ifdef METAL
    frag_color.xyz = frag_color.xyz * ( emissive + total_diffuse + ambient_light_color * ambient + total_specular );
#else
    frag_color.xyz = frag_color.xyz * ( emissive + ambient_light_color * ambient + total_diffuse ) + total_specular;
#endif
	
#ifdef USE_ENVMAP
	vec3 reflect_vec;
#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )
    vec3 camera_to_vert = normalize( vertex_pos_w - camera_pos_w );
    vec3 world_normal   = normalize( vec3( vec4(norm_normal_c, 0.0) * view_mat ) );
    if ( use_refraction ) {
        reflect_vec = refract(camera_to_vert, world_normal, refraction_ratio );
    }
    else {
        reflect_vec = reflect(camera_to_vert, world_normal );
    }
#else
    reflect_vec = reflect_c;
#endif
    
#ifdef DOUBLE_SIDED
    float flip_normal = (-1.0 + 2.0 * float( gl_FrontFacing ) );
    vec4 cube_color = texture( env_map, flip_normal * vec3( flip_env_map * reflect_vec.x, reflect_vec.yz ) );
#else
    vec4 cube_color = texture( env_map, flip_env_map * reflect_vec );
#endif
#ifdef GAMMA_INPUT
    cube_color.xyz *= cube_color.xyz;
#endif
	if( combine == 1 ) {
		frag_color.xyz = mix( frag_color.xyz, cube_color.xyz, specular_strength * reflectivity );
	}
	else if( combine == 2 ) {
		frag_color.xyz += cube_color.xyz * specular_strength * reflectivity;
	}
	else {
		frag_color.xyz = mix( frag_color.xyz, frag_color.xyz * cube_color.xyz, specular_strength * reflectivity );
	}
#endif
    
#ifdef USE_SHADOWMAP
	float f_depth;
	vec3 shadow_color = vec3(1.0);
	for (int i = 0; i < MAX_SHADOWS; i++ ) {
		vec3 shadow_coord = shadow_coord_c[i].xyz / shadow_coord_c[i].w;
		bvec4 in_frustum_vec = bvec4(shadow_coord.x >= 0.0, shadow_coord.x <= 1.0, shadow_coord.y >= 0.0, shadow_coord.y <= 1.0 );
		bool in_frustum        = all( in_frustum_vec );
		bvec2 frustum_test_vec = bvec2( in_frustum, shadow_coord.z <= 1.0 );
		bool frustum_test      = all( frustum_test_vec );
		if( frustum_test ) {
			shadow_coord.z += shadow_bias[i];
            
            #if defined( SHADOW_TYPE_PCF )
                float shadow             = 0.0;
                const float shadow_delta = 1.0 / 9.0;
                float x_offset           = 1.0 / shadow_map_size[i].x;
                float y_offset           = 1.0 / shadow_map_size[i].y;
                float dx0                = -1.25 * x_offset;
                float dy0                = -1.25 * y_offset;
                float dx1                =  1.25 * x_offset;
                float dy1                =  1.25 * y_offset;
            
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(dx0, dy0)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(0.0, dy0)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(dx1, dy0)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(dx0, 0.0)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(0.0, 0.0)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(dx1, 0.0)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(dx0, dy1)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(0.0, dy1)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                f_depth = unpackDepth(texture( shadow_map[i], shadow_coord.xy + vec2(dx1, dy1)) );
                if( f_depth < shadow_coord.z )
                    shadow += shadow_delta;
                shadow_color = shadow_color * vec3( 1.0 - shadow_darkness[i] * shadow );
            
            #elif defined( SHADOW_TYPE_PCF_SOFT )
                float shadow             = 0.0;
                const float shadow_delta = 1.0 / 9.0;
                float x_offset           = 1.0 / shadow_map_size[i].x;
                float y_offset           = 1.0 / shadow_map_size[i].y;
                float dx0                = -1.0 * x_offset;
                float dy0                = -1.0 * y_offset;
                float dx1                =  1.0 * x_offset;
                float dy1                =  1.0 * y_offset;
                mat3 shadow_kernel;
                mat3 depth_kernel;
                depth_kernel[0][0] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx0, dx0) ));
                depth_kernel[0][1] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx0, 0.0) ));
                depth_kernel[0][2] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx0, dy1) ));
                depth_kernel[1][0] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(0.0, dy0) ));
                depth_kernel[1][1] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(0.0, 0.0) ));
                depth_kernel[1][2] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx0, dy1) ));
                depth_kernel[2][0] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx1, dy0) ));
                depth_kernel[2][1] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx1, 0.0) ));
                depth_kernel[2][2] = unpackDepth( texture( shadow_map[i], shadow_coord.xy + vec2(dx1, dy1) ));
                
                vec3 shadow_z = vec3(shadow_coord.z);
                shadow_kernel[0] = vec3( lessThan(depth_kernel[0], shadow_z) );
                shadow_kernel[0] *= vec3(0.25);
                shadow_kernel[1] = vec3( lessThan(depth_kernel[1], shadow_z) );
                shadow_kernel[1] *= vec3(0.25);
                shadow_kernel[2] = vec3( lessThan(depth_kernel[2], shadow_z) );
                shadow_kernel[2] *= vec3(0.25);
                
                vec2 fraction_coord = 1.0 - fract(shadow_coord.xy * shadow_map_size[i].xy );
                shadow_kernel[0] = mix( shadow_kernel[1], shadow_kernel[0], fraction_coord.x );
                shadow_kernel[1] = mix( shadow_kernel[2], shadow_kernel[1], fraction_coord.x );
                
                vec4 shadow_values;
                shadow_values.x = mix( shadow_kernel[0][1], shadow_kernel[0][0], fraction_coord.y );
                shadow_values.y = mix( shadow_kernel[0][2], shadow_kernel[0][1], fraction_coord.y );
                shadow_values.z = mix( shadow_kernel[1][1], shadow_kernel[1][0], fraction_coord.y );
                shadow_values.w = mix( shadow_kernel[1][2], shadow_kernel[1][1], fraction_coord.y );
                shadow = dot(shadow_values, vec4(1.0));
                shadow_color = shadow_color * vec3( (1.0 - shadow_darkness[i] * shadow) );
                
            #else
                vec4 rgba_depth = texture( shadow_map[i], shadow_coord.xy );
                f_depth = unpackDepth( rgba_depth );
                if( f_depth < shadow_coord.z ) {
                    shadow_color = shadow_color * vec3( 1.0 - shadow_darkness[i] );
                }
            #endif
		}
	}
    
    #ifdef GAMMA_OUTPUT
        shadow_color *= shadow_color;
    #endif
        frag_color.xyz = frag_color.xyz * shadow_color;
    #endif
        
    #ifdef GAMMA_OUTPUT
        frag_color.xyz = sqrt(frag_color.xyz);
    #endif
	
    
    #ifdef USE_FOG
        #ifdef USE_LOGDEPTHBUF_EXT
            float depth = gl_FragDepthEXT / gl_FragCoord.w;
        #else
            float depth = gl_FragCoord.z / gl_FragCoord.w;
        #endif
            
        #ifdef FOG_EXP2
            const float LOG2 = 1.442695;
            float fog_factor = exp2( -fog_density * fog_density * depth * depth * LOG2 );
            fog_factor = 1.0 - clamp( fog_factor, 0.0, 1.0 );
        #else
            float fog_factor = smoothstep( fog_near, fog_far, depth );
        #endif
        
        frag_color = mix( frag_color, vec4( fog_color, frag_color.w ), fog_factor );
    #endif
}