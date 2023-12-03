#pragma once

#include <GL/glew.h>
#include <vector>

class VBO
{
public:
    const GLuint getID() const { return id; }

    virtual const int getNumVertices() const = 0;

    virtual void createVBO() = 0;

    virtual void deleteVBO() = 0;

protected:
    VBO();
    GLuint id = 0;
    std::vector<float> vertexData;
};

