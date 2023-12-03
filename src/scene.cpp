#include <stdexcept>
#include "scene.h"
#include "utils/sceneparser.h"

Scene::Scene() : camera(800, 600) {
    render_width = 800;
    render_height = 600;
}

Scene::Scene(int width, int height, RenderData &metaData) :
    camera(width, height, metaData.cameraData) {
    render_width = width;
    render_height = height;
    renderData = metaData;
//    texture_maps = std::unordered_map<std::string, const Texture>();

//    for (RenderShapeData& e : metaData.shapes) {
//        SceneFileMap *fileMap = &e.primitive.material.textureMap;
//        if (fileMap->filename == "" || texture_maps.contains(fileMap->filename)) continue;
//        texture_maps.insert({
//            fileMap->filename,
//            Texture(fileMap->filename, fileMap->repeatU, fileMap->repeatV)
//        });
//    }
}

void Scene::translateCam(float forward, float right, float up) {
    camera.translate(forward, right, up);
}

void Scene::rotateCam(float deltaX, float deltaY) {
    camera.rotate(deltaX, deltaY);
}

void Scene::updateCamProjMatrix(int width, int height, float near, float far) {
    camera.updateProjMatrix(width, height, near, far);
}

const int& Scene::width() const {
    // Optional TODO: implement the getter or make your own design
    return render_width;
}

const int& Scene::height() const {
    // Optional TODO: implement the getter or make your own design
    return render_height;
}

const SceneGlobalData& Scene::getGlobalData() const {
    // Optional TODO: implement the getter or make your own design
    return renderData.globalData;
}

const Camera& Scene::getCamera() const {
    // Optional TODO: implement the getter or make your own design
    return camera;
}

const std::vector<RenderShapeData>& Scene::getShapes() const {
    return renderData.shapes;
}


const std::vector<SceneLightData>& Scene::getLights() const {
    return renderData.lights;
}

//const Texture& Scene::getTexture(const std::string& name) const {
//    return texture_maps.at(name);
//}
