#version 330 core

// Task 4: declare a vec3 object-space position variable, using
//         the `layout` and `in` keywords.
layout(location = 0) in vec3 objPos;
layout(location = 1) in vec3 objNorm;
layout(location = 2) in vec3 tangent;

// Task 5: declare `out` variables for the world-space position and normal,
//         to be passed to the fragment shader
out vec4 worldPos;
out vec3 worldNorm;
out vec3 worldTangent;

// Task 6: declare a uniform mat4 to store model matrix
uniform mat4 modelMat;
uniform mat3 normTrans;

// Task 7: declare uniform mat4's for the view and projection matrix
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    // Task 8: compute the world-space position and normal, then pass them to
    //         the fragment shader using the variables created in task 5

    // Recall that transforming normals requires obtaining the inverse-transpose of the model matrix!
    // In projects 5 and 6, consider the performance implications of performing this here.

    worldPos = modelMat * vec4(objPos, 1.0);
    worldNorm = normTrans * normalize(objNorm);
    worldTangent = normTrans * normalize(tangent);

    // Task 9: set gl_Position to the object space position transformed to clip space
    gl_Position = projMat * viewMat * worldPos;
}
