#include "Skyboxvao.h"

#include "debug.h"

SkyboxVAO::SkyboxVAO()
{

}

void SkyboxVAO::createVAO() {
    // fill with cube vertices
    std::vector<GLfloat> skyboxVertices = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    vbo.updateVertexData(skyboxVertices);

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

    // enable and define attribute 0 to store vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    // reset default binding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void SkyboxVAO::draw(GLuint id_cubemap) {
    // bind vertex array
    glBindVertexArray(id);

    // set active texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_cubemap);

    // disable depth test
    glDisable(GL_DEPTH_TEST);

    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vbo.getNumVertices());

    // re-enable depth test
    glEnable(GL_DEPTH_TEST);

    // set default bindings
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void SkyboxVAO::deleteVAO() {
    vbo.deleteVBO();
    glDeleteVertexArrays(1, &id);
    Debug::glErrorCheck();
}
