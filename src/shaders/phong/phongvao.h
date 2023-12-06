#pragma once

#include "shaders/vao.h"
#include "phongvbo.h"

class PhongVAO : public VAO
{
public:
    PhongVAO(Shape* unitShape);

    void updateVertexData(const std::vector<float>& data);

    void updateShapeData(int param1, int param2);

    void draw(std::vector<GLuint> texIDs);

    void createVAO();

    void deleteVAO();

private:
    PhongVBO vbo;
};

