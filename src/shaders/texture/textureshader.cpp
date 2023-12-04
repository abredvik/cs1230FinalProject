#include "textureshader.h"

#include "debug.h"
#include "settings.h"
#include "utils/shaderloader.h"

TextureShader::TextureShader()
{

}

void TextureShader::updateViewport(int width, int height, GLuint fbo, GLuint default_fbo) {
    screen_width = width;
    screen_height = height;
    defaultFBO = default_fbo;
}

void TextureShader::updateTexture(GLuint texID) {
    id_texture = texID;
}

void TextureShader::createShader(int width, int height, GLuint fbo) {
    screen_width = width;
    screen_height = height;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    id = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");

    // create VAO
    fullscreenQuad.createVAO();
}

void TextureShader::draw() {
    // bind default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

    // set viewport
    glViewport(0, 0, screen_width, screen_height);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind the shader program
    glUseProgram(id);

    // set texture
    glUniform1i(glGetUniformLocation(id, "tex"), 0);

    // set image dimensions
    glUniform1i(glGetUniformLocation(id, "width"), screen_width);
    glUniform1i(glGetUniformLocation(id, "height"), screen_height);

    // Task 32: Set your bool uniform on whether or not to filter the texture drawn
    glUniform1i(glGetUniformLocation(id, "perPixelFilter"), settings.perPixelFilter);
    glUniform1i(glGetUniformLocation(id, "kernelFilter"), settings.kernelBasedFilter);

    fullscreenQuad.draw(id_texture);

    // unbind program
    glUseProgram(0);

    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Debug::glErrorCheck();
}

void TextureShader::deleteShader() {
    fullscreenQuad.deleteVAO();
}
