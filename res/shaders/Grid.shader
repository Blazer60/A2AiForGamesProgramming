#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 colour;

out vec3 v_colour;

uniform mat4 u_mvp_matrix;

void main()
{
    v_colour = colour;
    gl_Position = u_mvp_matrix * position;
}

#shader fragment
#version 460 core

in vec3 v_colour;

void main()
{
    gl_FragColor = vec4(v_colour, 1.0);
}

