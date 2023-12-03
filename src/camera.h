#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
    Camera(int width, int height);
    Camera(int width, int height, const SceneCameraData& data);

    const glm::vec4& getPosition() const;

    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    const glm::mat4& getViewMatrix() const;

    const glm::mat4& getInvViewMatrix() const;

    const glm::mat4& getProjMatrix() const;

    void translate(float forward, float right, float up);

    void updatePos(glm::vec4 xyz);

    void rotate(float deltaX, float deltaY);

    void updateProjMatrix(int width, int height, float near, float far);

    const float getNear() const;
    const float getFar() const;

    // Returns the aspect ratio of the camera.
    const float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    const float getHeightAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    const float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    const float getAperture() const;
private:
    glm::vec3 u, v, w;
    glm::vec4 position;
    glm::mat4 viewMatrix;
    glm::mat4 invViewMatrix;
    glm::mat4 projMatrix;
    float aspectRatio;
    float heightAngle;
    float focalLength;
    float aperture;
    float nearPlane;
    float farPlane;

    void rotateAxes(float angleDeg, glm::vec3 axis);
    void updateViewMatrix();
};
