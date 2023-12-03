#pragma once

#include "shader.h"
#include "scene.h"
#include "phongvao.h"
#include "fbo.h"
#include <GL/glew.h>

class PhongShader : public Shader
{
public:
    PhongShader();

    const GLuint getTexID() const { return fbo.getTexID(); }

    void updateViewport(int width, int height, GLuint default_fbo);

    void updateScene(const Scene* scene) { currentScene = scene; }

    void updateShapeData(int param1, int param2);

    void createShader(int width, int height);

    void draw();

    void deleteShader();

private:
    const Scene* currentScene;

    PhongVAO vao_cube;
    PhongVAO vao_cone;
    PhongVAO vao_cylinder;
    PhongVAO vao_sphere;

    FBO fbo;

};

