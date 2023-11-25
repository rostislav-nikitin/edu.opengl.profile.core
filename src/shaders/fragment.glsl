#version 330 core

out vec4 out_color;
in vec3 color;
in vec2 tex_coord;

uniform sampler2D tex;

void main()
{
    //out_color = vec4(color, 1.0);
    //out_color = texture(tex, tex_coord) * vec4(color, 1.0);
    out_color = texture(tex, tex_coord);
}
