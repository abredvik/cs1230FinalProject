#include "phongvbo.h"
#include "debug.h"

PhongVBO::PhongVBO(Shape* unitShape)
{
    if (unitShape != nullptr) {
        shape = unitShape;
        vertexData = unitShape->generateShape();
    } else {
        shape = nullptr;
    }
}

void PhongVBO::createVBO() {
    // ensure nothing is bound
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate and bind VBO
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);

    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    // reset bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

const int PhongVBO::getNumVertices() const {
    return vertexData.size() / 9;
}

void PhongVBO::updateVertexData(const std::vector<float>& data) {
    // copy data
    vertexData = data;

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, id);

    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    // reset bindings
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

void PhongVBO::updateShapeData(int param1, int param2) {
    if (!id || shape == nullptr) return;

    shape->updateParams(param1, param2);
    vertexData = shape->generateShape();

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, id);

    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    // reset bindings
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

void PhongVBO::deleteVBO() {
    glDeleteBuffers(1, &id);
    Debug::glErrorCheck();
}
