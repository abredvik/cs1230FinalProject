#pragma once

#include "../shader.h"
#include "scene.h"
#include "phongvao.h"
#include "utils/terraingenerator.h"
#include <GL/glew.h>

class PhongShader : public Shader
{
public:
    PhongShader();

    void updateViewport(int width, int height, GLuint fbo, GLuint default_fbo);

    void updateScene(const Scene* scene) { currentScene = scene; }

    void updateShapeData(int param1, int param2);

    void createShader(int width, int height, GLuint fbo);

    GLuint sendUniformTexMap(const SceneFileMap& tmap, std::string mapType, int texSlot);

    void draw();

    void deleteShader();

private:
    const Scene* currentScene;

    PhongVAO vao_cube;
    PhongVAO vao_cone;
    PhongVAO vao_cylinder;
    PhongVAO vao_sphere;

    TerrainGenerator generator;
    PhongVAO vao_terrain;

    GLuint id_fbo;

    std::unordered_map<std::string, GLuint> str2texmapID;

};

