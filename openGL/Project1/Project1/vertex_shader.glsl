#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTex;

out vec4 vertexColor;
out vec2 texCord;
uniform float newValue;
uniform mat4 Trans;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    //gl_Position = Trans*vec4(aPos.x + newValue, aPos.y, aPos.z, 1.0);
    gl_Position = Projection*View*Model*vec4(aPos,1.0);
    vertexColor = vec4(aPos,1.0);
    texCord = vec2(aTex.x, aTex.y);
}

