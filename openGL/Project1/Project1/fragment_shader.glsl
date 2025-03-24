#version 330 core
in vec4 vertexColor;
in vec2 texCord;
out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

uniform float alpha;

void main()
{ 
    vec2 flipC = vec2(1.0-texCord.x, texCord.y);
    //FragColor = mix( texture(tex1, texCord), texture(tex2, flipC), alpha) ;
    FragColor = mix(texture(tex1, texCord), texture(tex2,texCord), alpha);
}