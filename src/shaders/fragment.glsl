#version 330 core

out vec4 out_color;
in vec3 color;
in vec2 tex_coord;

uniform sampler2D tex_box;
uniform sampler2D tex_face;

void main()
{
    //out_color = vec4(color, 1.0);
    //out_color = texture(tex, tex_coord) * vec4(color, 1.0);
    //out_color = texture(tex_box, tex_coord);
    //out_color = mix(texture(tex_box, tex_coord), texture(tex_face, tex_coord), 0.2);
    out_color = mix(texture(tex_box, tex_coord), texture(tex_face, vec2(1.0 - tex_coord.x, tex_coord.y)), 0.2);
}
