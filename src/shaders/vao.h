#pragma once

#include <GL/glew.h>

class VAO
{
public:
    virtual void createVAO() = 0;

    virtual void deleteVAO() = 0;

protected:
    VAO();
    GLuint id = 0;
};
