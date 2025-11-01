#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D Tex[3];
uniform mat4 trans[3];
void main()
{
    vec4 a = texture(Tex[0],TexCoord);
    //vec4 flash = texture(Tex[1],TexCoord);
    //vec4 room = texture(Tex[0],(trans[0]*vec4(TexCoord.xy,0,1)).xy);
    vec4 b = texture(Tex[1],(trans[1]*vec4(TexCoord.xy,0,1)).xy);
    vec4 c = texture(Tex[2],(trans[2]*vec4(TexCoord.xy,0,1)).xy);
    if(c.a == 1)
        FragColor = c;
    else{
        if(b.a == 1)
            FragColor = b;
        else
            FragColor = a;
        }
}