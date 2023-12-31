#version 330 core

layout (location = 0) in vec3 in_pos;
//layout (location = 1) in vec3 in_color;
layout (location = 1) in vec2 in_tex_coord;

out vec3 color;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 transform;

void main()
{
    //gl_Position = transform * vec4(in_pos, 1.0);
    gl_Position = projection * view * model * vec4(in_pos, 1.0);
//    color = in_color;
	tex_coord = in_tex_coord;
}
