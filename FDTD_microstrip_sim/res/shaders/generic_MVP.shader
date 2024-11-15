#shader vertex
#version 430

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    gl_Position = proj * view * worldPosition;
}

#shader fragment
#version 430

out vec4 FragColor;
uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}