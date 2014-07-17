#version 410 core
#define USE_FOG
#define FOG_EXP2

layout (location = 0) in vec3 vertex_pos_m;
layout (location = 1) in vec3 vertex_normal_m;
layout (location = 2) in vec3 vertex_uv_m;

uniform mat4 model_mat;
uniform mat4 model_view_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat3 normal_mat;
uniform vec3 camera_pos_w;

out vec3 normal_c;
out vec3 eye_direction_c;

void main() {
	eye_direction_c   = vec3(0.0) - ( model_view_mat * vec4(vertex_pos_m, 1.0) ).xyz;
	normal_c          = normal_mat * vec3(vertex_normal_m);
	gl_Position       = projection_mat * model_view_mat * vec4(vertex_pos_m, 1.0);
}