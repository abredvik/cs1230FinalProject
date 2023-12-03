#include "texturevbo.h"
#include "debug.h"

TextureVBO::TextureVBO()
{

}

void TextureVBO::updateVertexData(const std::vector<float>& data) {
    // update vertex data
    vertexData = data;

    if (!id) return;

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, id);

    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    // reset bindings
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Debug::glErrorCheck();
}

void TextureVBO::createVBO() {
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

const int TextureVBO::getNumVertices() const {
    return vertexData.size() / 5;
}

void TextureVBO::deleteVBO() {
    glDeleteBuffers(1, &id);
    Debug::glErrorCheck();
}
