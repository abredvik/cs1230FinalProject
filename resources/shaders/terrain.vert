#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inColor;

out vec4 norm;
out vec3 color;
out vec3 lightDir;

uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    norm  = transpose(inverse(viewMat)) *  vec4(normal, 0.0);
    color = inColor;
    lightDir = normalize(vec3(viewMat * vec4(1, 0, 1, 0)));
    gl_Position = projMat * viewMat * vec4(vertex, 1.0);
}
