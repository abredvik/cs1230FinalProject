#include "bezier.h"
#include "glm/fwd.hpp"
#include <iostream>

//Bezier::Bezier()
//{

//}

//glm::vec4 Bezier::findCurvePoint(std::vector<glm::vec4> controlPoints, float t) {

//    if (controlPoints.size() == 1) {

//        return controlPoints[0];

//    }

//    else {
//        std::vector<glm::vec4> newPoints = std::vector<glm::vec4>(controlPoints.size() - 1);

//        for (int i = 0; i < newPoints.size(); i++) {

//            glm::vec4 newPoint;

////            newPoint = (1 - t) * controlPoints[i] + t * controlPoints[i + 1];

//                        float x = (1 - t) * controlPoints[i].x + t * controlPoints[i + 1].x;
//                        float y = (1 - t) * controlPoints[i].y + t * controlPoints[i + 1].y;
//                        float z = (1 - t) * controlPoints[i].z + t * controlPoints[i + 1].z;

//                        newPoints[i] = glm::vec4(x, y, z, 1.0f);

//            newPoints[i] = newPoint;

//            findCurvePoint(newPoints, t);
//        }
//    }
//}

void Bezier::linearInterpolation(glm::vec3& dest, const glm::vec3& a, const glm::vec3& b, const float t) {

//    dest.x = a.x + (b.x - a.x) * t;
//    dest.y = a.y + (b.y - a.y) * t;

    dest = a + (b - a) * t;
}

void Bezier::findCurvePoint(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3 c, const glm::vec3& d, const float t) {

    glm::vec3 ab, bc, cd, abbc, bccd;
    Bezier::linearInterpolation(ab, a, b, t);
    Bezier::linearInterpolation(bc, b, c, t);
    Bezier::linearInterpolation(cd, c, d, t);
    Bezier::linearInterpolation(abbc, ab, bc, t);
    Bezier::linearInterpolation(bccd, bc, cd, t);
    Bezier::linearInterpolation(dest, abbc, bccd, t);
}

std::vector<glm::vec4> Bezier::testPoints() {

    glm::vec3 controlPointA = glm::vec3(-40, 20, 20);
    glm::vec3 controlPointB = glm::vec3(0, 1, 0);
    glm::vec3 controlPointC = glm::vec3(1, 0.5, 0);
    glm::vec3 controlPointD = glm::vec3(50, 50, 50);

    std::vector<glm::vec4> pathPoints;

    for (int i = 3000; i < 5000; i++) {
        // float t = static_cast<float>(i)/999.0f;
        float t = pow(static_cast<float>(i) / 4999.0f, 2.0f);
        glm::vec3 p;
        Bezier::findCurvePoint(p, controlPointA, controlPointB, controlPointC, controlPointD, t);
        std::cout << "pathPoint.x: " << p.x << " pathPoint.y: " << p.y << " pathPoint.z: " << p.z << std::endl;

        pathPoints.push_back(glm::vec4(p, 1));

        // if you know the tangent vector for the previous point and you calculate the tangent for the current point;
        // the more the dot product between those two the more you can rotate the camera

        // fixed downward path for the look vector
        // another thing is try to make the look vector tangential to the curve
    }

    return pathPoints;
}
