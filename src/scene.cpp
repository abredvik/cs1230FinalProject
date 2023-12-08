#include <filesystem>
#include <stdexcept>
#include "scene.h"
#include "utils/sceneparser.h"

Scene::Scene() : camera(800, 600) {
    render_width = 800;
    render_height = 600;

    // define default scene for terrain generation
    glm::vec4 position(-2.f, 4.f, 4.f, 1.f);
    renderData = RenderData {
        .globalData = SceneGlobalData {
            .ka = 0.5f, .kd = 0.5f, .ks = 0.5f, .kt = 0.f
        },
        .cameraData = SceneCameraData {
            .pos = position,
            .look = -position,
            .up = glm::vec4(0.f, 1.f, 0.f, 0.f),
            .heightAngle = glm::radians(30.f),
            .aperture = 0.f,   // not used
            .focalLength = 0.f // not used
        },
        .lights = {
            SceneLightData {
                .id = 0, // not used
                .type = LightType::LIGHT_DIRECTIONAL,
                .color = glm::vec4(1.f),
                .function = glm::vec3(0.f), // not used
                .pos = glm::vec4(0.f), // not used
                .dir = glm::vec4(0.f, -1.f, 1.f, 0.f),
                .penumbra = 0.f, // not used
                .angle = 0.f, // not used
                .width = 0.f, // not used
                .height = 0.f // not used
            }
        },
        .shapes = {
            RenderShapeData {
                .primitive = ScenePrimitive {
                    .type = PrimitiveType::PRIMITIVE_MESH,
                    .material = SceneMaterial {
                        .cAmbient = glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
                        .cDiffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
                        .cSpecular = glm::vec4(1.f),
                        .shininess = 1,
                        .cReflective = glm::vec4(0.f), // not used
                        .cTransparent = glm::vec4(0.f), // not used
                        .ior = 0, // not used
                        .textureMaps = {
                            SceneFileMap(true, "./tr1rock1.jpg", 1.f, 1.f),
                            SceneFileMap(true, "./grassyDirt.jpg", 1.f, 1.f),
                        },
                        .blend = 1.f,
                        .cEmissive = glm::vec4(0.f), // not used
                        .bumpMaps = {
                            SceneFileMap(true, "./rockBump.jpg", 1.f, 1.f),
                            SceneFileMap(true, "./grassBump.jpg", 1.f, 1.f),
                        }
                    },
                    .meshfile = "terrain" // not used
                },
               .ctm = glm::mat4(1.f),
               .inv_ctm = glm::mat4(1.f),
               .normalTransform = glm::mat3(1.f)
            },
        }
    };
    camera = Camera(800, 600, renderData.cameraData);
}

Scene::Scene(int width, int height, RenderData &metaData) :
    camera(width, height, metaData.cameraData) {
    render_width = width;
    render_height = height;
    renderData = metaData;
}

void Scene::translateCam(float forward, float right, float up) {
    camera.translate(forward, right, up);
}

void Scene::updateCamLookUp(glm::vec3 newLook, glm::vec3 newUp) {
    camera.updateLookUp(newLook, newUp);
}

void Scene::updateCamPos(glm::vec4 xyz) {
    camera.updatePos(xyz);
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
