#pragma once

#include "../shader.h"
#include "scene.h"
#include "phongvao.h"
#include <GL/glew.h>

class PhongShader : public Shader
{
public:
    PhongShader();

    void updateViewport(int width, int height, GLuint fbo, GLuint default_fbo);

    void updateScene(const Scene* scene) { currentScene = scene; }

    void updateShapeData(int param1, int param2);

    void createShader(int width, int height, GLuint fbo);

    void draw();

    void deleteShader();

private:
    const Scene* currentScene;

    PhongVAO vao_cube;
    PhongVAO vao_cone;
    PhongVAO vao_cylinder;
    PhongVAO vao_sphere;

    GLuint id_fbo;

};

