#shader vertex
#version 430

layout (location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 proj;

uniform vec4 col;  
out vec4 color;

void main()
{
	gl_Position = proj * view * vec4(pos.x, pos.y, pos.z, 1.0f);
	color = col;
}

#shader fragment
#version 430


out vec4 FragColor;
in vec4 color;

void main()
{
	FragColor = color;
}