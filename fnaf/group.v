#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 TexCoord;
uniform mat4 otto;
uniform mat4 view;

void main()
{
    TexCoord = aTex;
    gl_Position = otto *view* vec4(aPos.x, aPos.y, aPos.z, 1.0);
}