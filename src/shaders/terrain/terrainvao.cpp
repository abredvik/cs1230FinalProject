#include "terrainvao.h"
#include "debug.h"

TerrainVAO::TerrainVAO()
{

}

void TerrainVAO::createVAO() {
    // create VBO
    vbo.createVBO();

    // fill VBO with terrain data
    vbo.updateVertexData(generator.generateTerrain());

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

    // Enable and define attribute 2 to store vertex colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    // reset bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

void TerrainVAO::draw() {
    // bind the VAO
    glBindVertexArray(id);

    // Execute draw call
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, vbo.getNumVertices());

    // clear binding
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void TerrainVAO::deleteVAO() {
    vbo.deleteVBO();
    glDeleteVertexArrays(1, &id);
    Debug::glErrorCheck();
}
