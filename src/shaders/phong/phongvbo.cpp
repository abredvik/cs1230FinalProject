#include "phongvbo.h"
#include "debug.h"

PhongVBO::PhongVBO(Shape* unitShape)
{
    shape = unitShape;
    vertexData = unitShape->generateShape();
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
    return shape->getNumVertices();
}

void PhongVBO::updateShapeData(int param1, int param2) {
    if (!id) return;

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
