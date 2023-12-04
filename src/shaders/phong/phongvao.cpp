#include "phongvao.h"
#include "debug.h"

PhongVAO::PhongVAO(Shape* unitShape) : vbo(unitShape)
{

}

void PhongVAO::createVAO() {
    // create VBO
    vbo.createVBO();

    // clear bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate, and bind vao
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);

    // bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo.getID());

    // Enable and define attribute 0 to store vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));

    // Enable and define attribute 1 to store vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    // reset bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

void PhongVAO::updateShapeData(int param1, int param2) {
    vbo.updateShapeData(param1, param2);
}

void PhongVAO::draw() {
    // bind the VAO
    glBindVertexArray(id);

    // Execute draw call
    glDrawArrays(GL_TRIANGLES, 0, vbo.getNumVertices());

    // clear binding
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void PhongVAO::deleteVAO() {
    vbo.deleteVBO();
    glDeleteVertexArrays(1, &id);
    Debug::glErrorCheck();
}
