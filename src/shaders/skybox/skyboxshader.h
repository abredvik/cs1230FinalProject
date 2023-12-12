#pragma once

#include "../shader.h"
#include "scene.h"
#include "skyboxvao.h"
#include <string>

class SkyboxShader : public Shader
{
public:
    SkyboxShader();

    void updateViewport(int width, int height, GLuint fbo, GLuint default_fbo);

    void updateScene(const Scene* scene) { currentScene = scene; }

    void updateCubeMap(GLuint texID);

    void createShader(int width, int height, GLuint fbo);

    void draw();

    void deleteShader();

private:
    const Scene* currentScene;

    GLuint id_cubemap;
    SkyboxVAO vao;

    GLuint id_fbo;

    std::string skyboxPath = "textures/skybox.png";
};
