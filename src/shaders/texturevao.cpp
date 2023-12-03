#include "texturevao.h"

#include "debug.h"

TextureVAO::TextureVAO()
{

}

void TextureVAO::createVAO() {
    static const std::vector<GLfloat> fullscreen_quad_data =
        { //     POSITIONS    //
         -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    vbo.updateVertexData(fullscreen_quad_data);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

    // enable and define attribute 1 to store uv coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // reset default binding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void TextureVAO::draw(GLuint texture) {
    // bind vertex array
    glBindVertexArray(id);

    // set active texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vbo.getNumVertices());

    // set default bindings
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void TextureVAO::deleteVAO() {
    vbo.deleteVBO();
    glDeleteVertexArrays(1, &id);
    Debug::glErrorCheck();
}
