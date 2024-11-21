#shader vertex
#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aOffset;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float sizeX;
uniform float sizeZ;

void main()
{
    vec3 scaledPos = aPos;
    scaledPos.x *= sizeX;
    scaledPos.z *= sizeZ;

    vec4 worldPosition = model * (vec4(scaledPos, 1.0) + vec4(aOffset, 0.0));
    gl_Position = proj * view * worldPosition;

    vertexColor = aColor;
}

#shader fragment
#version 430

in vec3 vertexColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}