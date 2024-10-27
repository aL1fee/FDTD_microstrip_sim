#shader vertex
#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

uniform mat4 view;
uniform mat4 proj;

out vec3 color;

void main()
{
	gl_Position = proj * view * vec4(pos.x, pos.y, pos.z, 1.0f);
	color = col;
}

#shader fragment
#version 430

out vec4 FragColor;
in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
}