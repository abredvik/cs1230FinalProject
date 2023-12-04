#include "terrainshader.h"
#include "utils/shaderloader.h"
#include "debug.h"

TerrainShader::TerrainShader()
{

}

void TerrainShader::updateViewport(int width, int height, GLuint fbo, GLuint default_fbo) {
    screen_width = width;
    screen_height = height;
    defaultFBO = default_fbo;
    id_fbo = fbo;
}

void TerrainShader::createShader(int width, int height, GLuint fbo) {
    screen_width = width;
    screen_height = height;
    id_fbo = fbo;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    id = ShaderLoader::createShaderProgram(":/resources/shaders/terrain.vert", ":/resources/shaders/terrain.frag");

    // create VAO
    vao.createVAO();
}

void TerrainShader::draw() {
    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, id_fbo);

    // update viewport
    glViewport(0, 0, screen_width, screen_height);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind the shader program
    glUseProgram(id);

    // send view and projection matrices to vertex shader
    const Camera& cam = currentScene->getCamera();
    glUniformMatrix4fv(glGetUniformLocation(id, "viewMat"), 1, GL_FALSE, &cam.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(id, "projMat"), 1, GL_FALSE, &cam.getProjMatrix()[0][0]);

    // draw the terrain
    vao.draw();

    // unbind program
    glUseProgram(0);

    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Debug::glErrorCheck();
}

void TerrainShader::deleteShader() {
    vao.deleteVAO();
}
