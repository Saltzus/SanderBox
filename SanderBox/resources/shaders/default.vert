#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec3 vertexPos;

uniform vec4 color;

uniform mat4 model;
uniform mat4 camera;

void main()
{

    // note that we read the multiplication from right to left
    gl_Position = camera * model * vec4(aPos, 1.0);
    ourColor = color;
    vertexPos = aPos;
}