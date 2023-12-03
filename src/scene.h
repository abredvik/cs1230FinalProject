#pragma once

#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "camera.h"
#include <unordered_map>
#include <GL/glew.h>

// A class representing a scene to be ray-traced

// Feel free to make your own design choices for RayTraceScene, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Scene
{
public:
    Scene();
    Scene(int width, int height, RenderData &metaData);

    // The getter of the width of the scene
    const int& width() const;

    // The getter of the height of the scene
    const int& height() const;

    // The getter of the global data of the scene
    const SceneGlobalData& getGlobalData() const;

    // The getter of the shared pointer to the camera instance of the scene
    const Camera& getCamera() const;

    void translateCam(float forward, float right, float up);

    void rotateCam(float deltaX, float deltaY);

    void updateCamProjMatrix(int width, int height, float near, float far);

    const std::vector<RenderShapeData>& getShapes() const;

    const std::vector<SceneLightData>& getLights() const;

//    const Texture& getTexture(const std::string& name) const;

private:
    int render_width;
    int render_height;
    RenderData renderData;
    Camera camera;
//    std::unordered_map<std::string, const Texture> texture_maps;
};
