#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 TexCoord;
uniform mat4 trans;
void main()
{
    TexCoord = aTex;
    vec4 apos = trans * vec4(aPos.xyz,1);
    gl_Position = apos;
}