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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));

    // Enable and define attribute 1 to store vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    // Enable and define attribute 2 to store tangents
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    // reset bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

void PhongVAO::updateVertexData(const std::vector<float>& data) {
    vbo.updateVertexData(data);
}

void PhongVAO::updateShapeData(int param1, int param2) {
    vbo.updateShapeData(param1, param2);
}

void PhongVAO::draw(std::vector<GLuint> texIDs) {
    // bind vertex array
    glBindVertexArray(id);

    for (int i = 0; i < texIDs.size(); ++i) {
        // set active texture
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texIDs[i]);
    }

    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vbo.getNumVertices());

    // set default bindings
    for (int i = 0; i < texIDs.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void PhongVAO::deleteVAO() {
    vbo.deleteVBO();
    glDeleteVertexArrays(1, &id);
    Debug::glErrorCheck();
}
