#include <stdexcept>
#include "camera.h"
#include "settings.h"

Camera::Camera(int width, int height) {
    aspectRatio = (float)width / (float)height;
}

Camera::Camera(int width, int height, const SceneCameraData& data) {
    aperture = data.aperture;
    focalLength = data.focalLength;
    heightAngle = data.heightAngle;
    aspectRatio = (float)width / (float)height;
    position = data.pos;
    position[3] = 1.f;
    viewMatrix = glm::mat4(1.f);
    projMatrix = glm::mat4(1.f);
    updateProjMatrix(width, height, settings.nearPlane, settings.farPlane);

    glm::vec3 look3(data.look), up3(data.up), pos3(data.pos);
    w = -glm::normalize(look3);
    v = glm::normalize(up3 - (glm::dot(up3, w) * w));
    u = glm::cross(v, w);

    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    for (int i = 0; i < 3; ++i) {
        viewMatrix[i][0] = u[i];
        viewMatrix[i][1] = v[i];
        viewMatrix[i][2] = w[i];
    }

    glm::vec3 pos3(position);
    viewMatrix[3][0] = -glm::dot(pos3, u);
    viewMatrix[3][1] = -glm::dot(pos3, v);
    viewMatrix[3][2] = -glm::dot(pos3, w);

    invViewMatrix = glm::inverse(viewMatrix);
}

const glm::vec4& Camera::getPosition() const {
    return position;
}

const glm::mat4& Camera::getViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return viewMatrix;
}

const glm::mat4& Camera::getInvViewMatrix() const {
    return invViewMatrix;
}

const glm::mat4& Camera::getProjMatrix() const {
    return projMatrix;
}

const float Camera::getNear() const {
    return nearPlane;
}

const float Camera::getFar() const {
    return farPlane;
}

void Camera::translate(float forward, float right, float up) {
    glm::vec3 trans = up * glm::vec3(0.f, 1.f, 0.f) + right * u - forward * w;
    position += glm::vec4(trans, 0.f);

    glm::vec3 pos3(position);
    viewMatrix[3][0] = -glm::dot(pos3, u);
    viewMatrix[3][1] = -glm::dot(pos3, v);
    viewMatrix[3][2] = -glm::dot(pos3, w);
    invViewMatrix = glm::inverse(viewMatrix);
}

void Camera::updatePos(glm::vec4 xyz) {
    position = xyz;
    position[3] = 1.f;

    glm::vec3 pos3(position);
    viewMatrix[3][0] = -glm::dot(pos3, u);
    viewMatrix[3][1] = -glm::dot(pos3, v);
    viewMatrix[3][2] = -glm::dot(pos3, w);
    invViewMatrix = glm::inverse(viewMatrix);
}

void Camera::rotateAxes(float angleDeg, glm::vec3 axis) {
    glm::vec3 ax = glm::normalize(axis);
    float x = ax[0];
    float y = ax[1];
    float z = ax[2];
    float theta = glm::radians(angleDeg);
    float cosTheta = glm::cos(theta);
    float sinTheta = glm::sin(theta);

    glm::mat3 rodriguez(0.f);
    rodriguez[0][0] = cosTheta + x * x * (1.f - cosTheta);
    rodriguez[0][1] = x * y * (1.f - cosTheta) + z * sinTheta;
    rodriguez[0][2] = x * z * (1.f - cosTheta) - y * sinTheta;
    rodriguez[1][0] = x * y * (1.f - cosTheta) - z * sinTheta;
    rodriguez[1][1] = cosTheta + y * y * (1.f - cosTheta);
    rodriguez[1][2] = y * z * (1.f - cosTheta) + x * sinTheta;
    rodriguez[2][0] = x * z * (1.f - cosTheta) + y * sinTheta;
    rodriguez[2][1] = y * z * (1.f - cosTheta) - x * sinTheta;
    rodriguez[2][2] = cosTheta + z * z * (1.f - cosTheta);

    u = rodriguez * u;
    v = rodriguez * v;
    w = rodriguez * w;

    updateViewMatrix();
}

void Camera::rotate(float deltaX, float deltaY) {
    rotateAxes(deltaX / 10.f, glm::vec3(0.f, 1.f, 0.f));
    rotateAxes(deltaY / 10.f, u);
}

void Camera::updateProjMatrix(int width, int height, float near, float far) {
    glm::mat4 openGLtrans(1.f);// = {
//        1,  0,  0,  0,
//        0,  1,  0,  0,
//        0,  0, -2, -1,
//        0,  0,  0,  1
//    };

    openGLtrans[2][2] = -2.f;
    openGLtrans[3][2] = -1.f;

    aspectRatio = (float)width / (float)height;
    float tanThHo2 = glm::tan(heightAngle / 2.f);
    float tanThWo2 = tanThHo2 * aspectRatio;

    nearPlane = near;
    farPlane = far;

    float c = -near / far;
    glm::mat4 Mpp(1.f);// = {
//        1,  0,            0,          0,
//        0,  1,            0,          0,
//        0,  0,  1.f/(1.f+c), -c/(1.f+c),
//        0,  0,           -1,          0,
//    };

    Mpp[2][2] = 1.f / (1.f + c);
    Mpp[3][2] = -c / (1.f + c);
    Mpp[2][3] = -1.f;
    Mpp[3][3] = 0.f;

    glm::mat4 Sxyz(1.f);// = {
//        1.f/(far*tanThWo2), 0,        0, 0,
//        0, 1.f/(far*tanThHo2),        0, 0,
//        0,                   0, 1.f/far, 0,
//        0,                   0,       0, 1
//    };

    Sxyz[0][0] = 1.f / (far * tanThWo2);
    Sxyz[1][1] = 1.f / (far * tanThHo2);
    Sxyz[2][2] = 1.f / far;

    projMatrix = openGLtrans * Mpp * Sxyz;
}

const float Camera::getAspectRatio() const {
    // Optional TODO: implement the getter or make your own design
    return aspectRatio;
}

const float Camera::getHeightAngle() const {
    // Optional TODO: implement the getter or make your own design
    return heightAngle;
}

const float Camera::getFocalLength() const {
    // Optional TODO: implement the getter or make your own design
    return focalLength;
}

const float Camera::getAperture() const {
    // Optional TODO: implement the getter or make your own design
    return aperture;
}
