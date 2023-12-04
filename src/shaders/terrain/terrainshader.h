#pragma once

#include "scene.h"
#include "../shader.h"
#include "terrainvao.h"

class TerrainShader : public Shader
{
public:
    TerrainShader();

    void updateViewport(int width, int height, GLuint fbo, GLuint default_fbo);

    void updateScene(const Scene* scene) { currentScene = scene; }

    void createShader(int width, int height, GLuint fbo);

    void draw();

    void deleteShader();

private:
    const Scene* currentScene;
    TerrainVAO vao;
    GLuint id_fbo;
};
