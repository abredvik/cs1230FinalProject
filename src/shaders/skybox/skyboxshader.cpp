#include "Skyboxshader.h"

#include "debug.h"
#include "qimage.h"
#include "utils/shaderloader.h"

SkyboxShader::SkyboxShader()
{

}

void SkyboxShader::updateViewport(int width, int height, GLuint fbo, GLuint default_fbo) {
    screen_width = width;
    screen_height = height;
    id_fbo = fbo;
    defaultFBO = default_fbo;
}

void SkyboxShader::updateCubeMap(GLuint texID) {
    id_cubemap = texID;
}

QImage createSubImage(QImage* image, const QRect & rect) {
    size_t offset = rect.x() * image->depth() / 8
                    + rect.y() * image->bytesPerLine();
    return QImage(image->bits() + offset, rect.width(), rect.height(),
                  image->bytesPerLine(), image->format());
}

void SkyboxShader::createShader(int width, int height, GLuint fbo) {
    screen_width = width;
    screen_height = height;
    id_fbo = fbo;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    id = ShaderLoader::createShaderProgram(":/resources/shaders/skybox.vert", ":/resources/shaders/skybox.frag");

    // load in texture map
    QImage img = QImage(QString(skyboxPath.c_str()));
    img = img.convertToFormat(QImage::Format_RGBA8888);
    int squareWidth = img.width() / 4, squareHeight = img.height() / 3;
    if (squareWidth != squareHeight) printf("PANIC: image dimensions not square\n");

    // Task 3: Generate kitten texture
    glGenTextures(1, &id_cubemap);

    // Task 9: Set the active texture slot to texture slot 0
    glActiveTexture(GL_TEXTURE0);

    // Task 4: Bind kitten texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_cubemap);

    int faceIndex = 0;
    std::vector<GLuint> faces = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,	// left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,	// top
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,	// back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // bottom
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,	// right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,	// front
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 && j != 1) continue;
            QImage square = img.copy(i * squareWidth, j * squareHeight, squareWidth, squareHeight);
            glTexImage2D(faces[faceIndex++], 0, GL_RGBA, squareWidth, squareHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, square.bits());
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // create VAO
    vao.createVAO();
}

void SkyboxShader::draw() {
    // bind default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, id_fbo);

    // set viewport
    glViewport(0, 0, screen_width, screen_height);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind the shader program
    glUseProgram(id);

    // set cubemap
    glUniform1i(glGetUniformLocation(id, "skybox"), 0);

    // set view and projection matrices
    const Camera& cam = currentScene->getCamera();
    glm::mat4 view = glm::mat4(glm::mat3(cam.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(id, "projMat"), 1, GL_FALSE, &cam.getProjMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(id, "viewMat"), 1, GL_FALSE, &view[0][0]);

    // draw
    vao.draw(id_cubemap);

    // unbind program
    glUseProgram(0);

    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Debug::glErrorCheck();
}

void SkyboxShader::deleteShader() {
    vao.deleteVAO();
}
