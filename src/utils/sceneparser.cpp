#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

glm::mat4 getGlmMatrix(SceneTransformation* transformation) {
    switch (transformation->type) {
    case TransformationType::TRANSFORMATION_TRANSLATE:
        return glm::translate(transformation->translate);
    case TransformationType::TRANSFORMATION_SCALE:
        return glm::scale(transformation->scale);
    case TransformationType::TRANSFORMATION_ROTATE:
        return glm::rotate(transformation->angle, transformation->rotate);
    case TransformationType::TRANSFORMATION_MATRIX:
        return transformation->matrix;
    default:
        return glm::mat4(1);
    }
}

void treeDFS(SceneNode* node, RenderData& renderData, glm::mat4 prev_ctm) {
    glm::mat4 newCTM(prev_ctm);
    for (SceneTransformation* transformation : node->transformations) {
        newCTM *= getGlmMatrix(transformation);
    }

    for (ScenePrimitive* primitive : node->primitives) {
        renderData.shapes.push_back(
            RenderShapeData {
                .primitive = *primitive,
                .ctm = newCTM,
                .inv_ctm = glm::inverse(newCTM),
                .normalTransform = glm::inverse(glm::transpose(glm::mat3(newCTM)))
            });
    }

    for (SceneLight* light : node->lights) {
        renderData.lights.push_back(
            SceneLightData {
                .id = light->id,
                .type = light->type,
                .color = light->color,
                .function = light->function,
                .pos = newCTM * glm::vec4(0.f, 0.f, 0.f, 1.f),
                .dir = newCTM * light->dir,
                .penumbra = light->penumbra,
                .angle = light->angle,
                .width = light->width,
                .height = light->height
            });
    }

    for (SceneNode* child : node->children) {
        treeDFS(child, renderData, newCTM);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // TODO: Use your Lab 5 code here
    // populate renderData with global data, and camera data;
    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();

    // populate renderData's list of primitives and their transforms.
    SceneNode *root = fileReader.getRootNode();
    renderData.shapes.clear();
    treeDFS(root, renderData, glm::mat4(1));

    return true;
}
