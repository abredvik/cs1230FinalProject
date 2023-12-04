#include "phongShader.h"
#include "../openglwrappers.h"
#include "shapes/cone.h"
#include "shapes/cube.h"
#include "shapes/cylinder.h"
#include "shapes/sphere.h"
#include "utils/shaderloader.h"

Cube unitCube = Cube();
Cone unitCone = Cone();
Cylinder unitCylinder = Cylinder();
Sphere unitSphere = Sphere();

PhongShader::PhongShader() : vao_cube(&unitCube), vao_cone(&unitCone), vao_cylinder(&unitCylinder), vao_sphere(&unitSphere), vao_terrain(nullptr)
{

}

void PhongShader::updateViewport(int width, int height, GLuint fbo, GLuint default_fbo) {
    //    if (fbo_width == width && fbo_height == height) return;
    defaultFBO = default_fbo;
    id_fbo = fbo;

    screen_width = width;
    screen_height = height;
}

void PhongShader::updateShapeData(int param1, int param2) {
    vao_cube.updateShapeData(param1, param2);
    vao_cone.updateShapeData(param1, param2);
    vao_cylinder.updateShapeData(param1, param2);
    vao_sphere.updateShapeData(param1, param2);
}

void PhongShader::createShader(int width, int height, GLuint fbo) {
    screen_width = width;
    screen_height = height;
    id_fbo = fbo;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    id = ShaderLoader::createShaderProgram(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");

    // create VAOs
    vao_cube.createVAO();
    vao_cone.createVAO();
    vao_cylinder.createVAO();
    vao_sphere.createVAO();
    vao_terrain.createVAO();
    vao_terrain.updateVertexData(generator.generateTerrain());
}

void PhongShader::drawTerrain() {
    // define RenderShapeData for the terrain
    RenderShapeData obj = RenderShapeData {
        .primitive = ScenePrimitive {
            .type = PrimitiveType::PRIMITIVE_MESH,
            .material = SceneMaterial {
                .cAmbient = glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
                .cDiffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
                .cSpecular = glm::vec4(1.f),
                .shininess = 10,
                .cReflective = glm::vec4(0.f), // not used
                .cTransparent = glm::vec4(0.f), // not used
                .ior = 0, // not used
                .textureMap = SceneFileMap{}, // not used
                .blend = 0.f, // not used
                .cEmissive = glm::vec4(0.f), // not used
                .bumpMap = SceneFileMap {} // not used
            },
            .meshfile = std::string()
        },
        .ctm = glm::mat4(1.f),
        .inv_ctm = glm::mat4(1.f),
        .normalTransform = glm::mat3(1.f)
    };

    // send uniforms to vertex shader
    sendUniformVert(id, currentScene, obj);

    // send uniforms to fragment shader
    sendUniformFrag(id, currentScene, obj);

    vao_terrain.draw();
}

void PhongShader::draw() {
    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, id_fbo);

    // update viewport
    glViewport(0, 0, screen_width, screen_height);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind the shader program
    glUseProgram(id);

    // draw terrain
    drawTerrain();

    for (const RenderShapeData& obj : currentScene->getShapes()) {

        // send uniforms to vertex shader
        sendUniformVert(id, currentScene, obj);

        // send uniforms to fragment shader
        sendUniformFrag(id, currentScene, obj);

        switch (obj.primitive.type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            vao_cube.draw();
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            vao_cone.draw();
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            vao_cylinder.draw();
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            vao_sphere.draw();
            break;
        case PrimitiveType::PRIMITIVE_MESH:
            continue;
        }

        Debug::glErrorCheck();
    }

    // unbind program
    glUseProgram(0);

    // unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PhongShader::deleteShader() {
    vao_cube.deleteVAO();
    vao_cone.deleteVAO();
    vao_cylinder.deleteVAO();
    vao_sphere.deleteVAO();
    vao_terrain.deleteVAO();
}
