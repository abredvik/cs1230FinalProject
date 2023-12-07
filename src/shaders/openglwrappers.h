#pragma once

#include "debug.h"
#include <scene.h>
#include <GL/glew.h>

void sendUniformMaterial(GLuint id_shader, const SceneMaterial& material) {
    //    struct Material {
    //        vec4 cAmbient;   // Ambient term
    //        vec4 cDiffuse;   // Diffuse term
    //        vec4 cSpecular;  // Specular term
    //        float shininess; // Specular exponent
    //    };
    glUniform4fv(glGetUniformLocation(id_shader, "material.cAmbient"), 1, &material.cAmbient[0]);
    glUniform4fv(glGetUniformLocation(id_shader, "material.cDiffuse"), 1, &material.cDiffuse[0]);
    glUniform4fv(glGetUniformLocation(id_shader, "material.cSpecular"), 1, &material.cSpecular[0]);
    glUniform1f(glGetUniformLocation(id_shader, "material.shininess"), material.shininess);
    glUniform1f(glGetUniformLocation(id_shader, "material.blend"), material.blend);
    Debug::glErrorCheck();
}

void sendUniformLights(GLuint id_shader, const std::vector<SceneLightData>& lights) {
    //    struct SceneLightData {
    //        int type;

    //        vec4 color;
    //        vec3 function; // Attenuation function

    //        vec4 pos; // Position with CTM applied (Not applicable to directional lights)
    //        vec4 dir; // Direction with CTM applied (Not applicable to point lights)

    //        float penumbra; // Only applicable to spot lights, in RADIANS
    //        float angle;    // Only applicable to spot lights, in RADIANS
    //    };

    //    uniform int numLights;
    //    uniform SceneLightData[8] lights;

    glUniform1i(glGetUniformLocation(id_shader, "numLights"), lights.size());
    for (int i = 0; i < lights.size(); ++i) {
        std::string index = "lights[" + std::to_string(i) + "]";
        GLint typeLoc = glGetUniformLocation(id_shader, (index + ".type").data());
        GLint colorLoc = glGetUniformLocation(id_shader, (index + ".color").data());
        GLint functionLoc = glGetUniformLocation(id_shader, (index + ".function").data());
        GLint posLoc = glGetUniformLocation(id_shader, (index + ".pos").data());
        GLint dirLoc = glGetUniformLocation(id_shader, (index + ".dir").data());
        GLint penumbraLoc = glGetUniformLocation(id_shader, (index + ".penumbra").data());
        GLint angleLoc = glGetUniformLocation(id_shader, (index + ".angle").data());

        glUniform1i(typeLoc, int(lights[i].type));
        glUniform4fv(colorLoc, 1, &lights[i].color[0]);
        glUniform3fv(functionLoc, 1, &lights[i].function[0]);
        glUniform4fv(posLoc, 1, &lights[i].pos[0]);
        glUniform4fv(dirLoc, 1, &lights[i].dir[0]);
        glUniform1f(penumbraLoc, lights[i].penumbra);
        glUniform1f(angleLoc, lights[i].angle);
    }
    Debug::glErrorCheck();
}

void sendUniformConstants(GLuint id_shader, const SceneGlobalData& gData) {
    //    uniform float Ka;
    //    uniform float Kd;
    //    uniform float Ks;
    glUniform1f(glGetUniformLocation(id_shader, "Ka"), gData.ka);
    glUniform1f(glGetUniformLocation(id_shader, "Kd"), gData.kd);
    glUniform1f(glGetUniformLocation(id_shader, "Ks"), gData.ks);
    Debug::glErrorCheck();
}

void sendUniformVert(GLuint id_shader, const Scene* scene, const RenderShapeData& obj) {
    // send model and normal transform matrices to vertex shader
    glUniformMatrix4fv(glGetUniformLocation(id_shader, "modelMat"), 1, GL_FALSE, &obj.ctm[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(id_shader, "normTrans"), 1, GL_FALSE, &obj.normalTransform[0][0]);

    // send view and projection matrices to vertex shader
    const Camera& cam = scene->getCamera();
    glUniformMatrix4fv(glGetUniformLocation(id_shader, "viewMat"), 1, GL_FALSE, &cam.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(id_shader, "projMat"), 1, GL_FALSE, &cam.getProjMatrix()[0][0]);

    Debug::glErrorCheck();
}

void sendUniformFrag(GLuint id_shader, const Scene* scene, const RenderShapeData& obj) {
    // send camera position to fragment shader
    glUniform4fv(glGetUniformLocation(id_shader, "camPos"), 1, &scene->getCamera().getPosition()[0]);

    // send material information to fragment shader
    sendUniformMaterial(id_shader, obj.primitive.material);

    // send light information to fragment shader
    sendUniformLights(id_shader, scene->getLights());

    // send global constants to fragment shader
    sendUniformConstants(id_shader, scene->getGlobalData());

    Debug::glErrorCheck();
}

