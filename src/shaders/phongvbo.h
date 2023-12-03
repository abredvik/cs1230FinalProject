#pragma once

#include "shaders/vbo.h"
#include "shapes/shape.h"
#include <GL/glew.h>
#include <vector>

class PhongVBO : public VBO
{
public:
    PhongVBO(Shape* unitShape);

    const int getNumVertices() const;

    void updateShapeData(int param1, int param2);

    void createVBO();

    void deleteVBO();

private:
    class Shape* shape = NULL;
};

