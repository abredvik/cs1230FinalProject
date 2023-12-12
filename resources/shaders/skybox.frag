#version 330 core

// Task 16: Create a UV coordinate in variable
in vec3 texCoord;

uniform samplerCube skybox;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.f);
    fragColor = texture(skybox, texCoord);
}
