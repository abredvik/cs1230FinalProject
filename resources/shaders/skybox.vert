#version 330 core

// Task 15: add a second layout variable representing a UV coordinate
layout (location = 0) in vec3 position;

// Task 16: create an "out" variable representing a UV coordinate
out vec3 texCoord;

uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    // Task 16: assign the UV layout variable to the UV "out" variable\
    texCoord = vec3(0.f);
    texCoord = vec3(position.x, position.y, -position.z);

    gl_Position = projMat * mat4(mat3(viewMat)) * vec4(position, 1.0);
}
