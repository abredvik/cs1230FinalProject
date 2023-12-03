#pragma once

#include <GL/glew.h>

class FBO
{
public:
    FBO();

    const GLuint getID() const { return id; }

    const GLuint getTexID() const { return id_texture; }

    void makeFBO(GLuint defaultFbo, GLsizei fbo_width, GLsizei fbo_height);

    void deleteFBO();

private:
    GLuint id;
    GLuint id_texture;
    GLuint id_renderbuffer;

    GLsizei width;
    GLsizei height;
};

