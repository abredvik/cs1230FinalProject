#pragma once

#include "skyboxvbo.h"
#include "../vao.h"

class SkyboxVAO : public VAO
{
public:
    SkyboxVAO();

    void draw(GLuint Skybox);

    void createVAO();

    void deleteVAO();

private:
    SkyboxVBO vbo;
};

