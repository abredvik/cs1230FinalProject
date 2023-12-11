#include "phongShader.h"
#include "../openglwrappers.h"
#include "qimage.h"
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

GLuint PhongShader::sendUniformTexMap(const SceneFileMap& tmap, std::string mapType, int texSlot) {
    // check if texture map is used
    if (!tmap.isUsed) {
        return 0; // return empty texture ID
    }

    // check if we already loaded the file
    if (!str2texmapID.contains(tmap.filename)) {
        // load in texture map
        QImage img = QImage(QString(tmap.filename.c_str()));
        img = img.convertToFormat(QImage::Format_RGBA8888).mirrored();

        // initialize id
        str2texmapID[tmap.filename] = 0;

        // Task 3: Generate kitten texture
        glGenTextures(1, &str2texmapID[tmap.filename]);

        // Task 9: Set the active texture slot to texture slot 0
        glActiveTexture(GL_TEXTURE0 + texSlot);

        // Task 4: Bind kitten texture
        glBindTexture(GL_TEXTURE_2D, str2texmapID[tmap.filename]);

        // Task 5: Load image into kitten texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

        // Task 6: Set min and mag filters' interpolation mode to linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Task 7: Unbind kitten texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // set the uniform
    glUniform1i(glGetUniformLocation(id, mapType.c_str()), texSlot);

    // return texture ID
    return str2texmapID[tmap.filename];
}

void PhongShader::draw() {
    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, id_fbo);

    // update viewport
    glViewport(0, 0, screen_width, screen_height);

    // clear buffer
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind the shader program
    glUseProgram(id);

    for (const RenderShapeData& obj : currentScene->getShapes()) {
        // list of textures to bind
        std::vector<GLuint> texIDs;
        int i = 0;

        // deal with texture maps
        const std::vector<SceneFileMap>& tmaps = obj.primitive.material.textureMaps;
        bool texMapUsed = false;
        for (; i < tmaps.size(); ++i) {
            // send texture map uniforms
            GLuint texID = sendUniformTexMap(tmaps[i], "texMaps[" + std::to_string(i) + "]", i);
            texIDs.push_back(texID);
            if (texID) texMapUsed = true;
        }

        // whether or not texture maps are used
        glUniform1i(glGetUniformLocation(id, "useTexMap"), texMapUsed);

        // deal with bump maps
        const std::vector<SceneFileMap>& bmaps = obj.primitive.material.bumpMaps;
        bool bumpMapUsed = false;
        for (int j = 0; j < bmaps.size(); ++j) {
            // send normal map uniforms
            GLuint texID = sendUniformTexMap(bmaps[j], "bumpMaps[" + std::to_string(j) + "]", i + j);
            texIDs.push_back(texID);
            if (texID) bumpMapUsed = true;
        }

        // whether or not bump maps are used
        glUniform1i(glGetUniformLocation(id, "useBumpMap"), bumpMapUsed);

        // send uniforms to vertex shader
        sendUniformVert(id, currentScene, obj);

        // send uniforms to fragment shader
        sendUniformFrag(id, currentScene, obj);

        switch (obj.primitive.type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            vao_cube.draw(texIDs);
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            vao_cone.draw(texIDs);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            vao_cylinder.draw(texIDs);
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            vao_sphere.draw(texIDs);
            break;
        case PrimitiveType::PRIMITIVE_MESH:
            vao_terrain.draw(texIDs);
            break;
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
