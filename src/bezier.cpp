#include "bezier.h"
#include "glm/fwd.hpp"
#include <iostream>

//Bezier::Bezier()
//{

//}

void Bezier::findCurvePoint(glm::vec4& dest, std::vector<glm::vec4>& controlPoints, float t) {

    if (controlPoints.size() == 1) {

//        std::cout << "X: " << controlPoints[0].x << std::endl;

        dest =  controlPoints[0];

    }

    else {
        std::vector<glm::vec4> newPoints = std::vector<glm::vec4>(controlPoints.size() - 1);

        for (int i = 0; i < newPoints.size(); i++) {

            glm::vec4 newPoint;

            newPoint = (1 - t) * controlPoints[i] + t * controlPoints[i + 1];

//                        float x = (1 - t) * controlPoints[i].x + t * controlPoints[i + 1].x;
//                        float y = (1 - t) * controlPoints[i].y + t * controlPoints[i + 1].y;
//                        float z = (1 - t) * controlPoints[i].z + t * controlPoints[i + 1].z;

                        // newPoints[i] = glm::vec3(x, y, z);

            newPoints[i] = newPoint;

            findCurvePoint(dest, newPoints, t);
        }
    }
}

void Bezier::linearInterpolation(glm::vec3& dest, const glm::vec3& a, const glm::vec3& b, const float t) {

//    dest.x = a.x + (b.x - a.x) * t;
//    dest.y = a.y + (b.y - a.y) * t;

    dest = a + (b - a) * t;
}

//void Bezier::findCurvePoint(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& e, const float t) {

//    glm::vec3 ab, bc, cd, abbc, bccd, abbcbccd,  ;
//    Bezier::linearInterpolation(ab, a, b, t);
//    Bezier::linearInterpolation(bc, b, c, t);
//    Bezier::linearInterpolation(cd, c, d, t);
//    Bezier::linearInterpolation(abbc, ab, bc, t);
//    Bezier::linearInterpolation(bccd, bc, cd, t);
//    Bezier::linearInterpolation(abbcbccd, abbc, bccd, t);

//    Bezier::linearInterpolation(dest, abbcbccd, bccdcdde, t);
//}

std::vector<glm::vec4> Bezier::testPoints() {

//    glm::vec3 controlPointA = glm::vec3(-40, 10, 20);
//    glm::vec3 controlPointB = glm::vec3(0, 10, 0);
//    glm::vec3 controlPointC = glm::vec3(1, 10, 0);
//    glm::vec3 controlPointD = glm::vec3(50, 10, -50);

//    glm::vec4 controlPointA = glm::vec4(-3.12691, 32, 20, 1);
//    glm::vec4 controlPointB = glm::vec4(15, 5, -20, 1);
//    glm::vec4 controlPointC = glm::vec4(45, 8, -80, 1);
//    glm::vec4 controlPointD = glm::vec4(75, 7, -126, 1);
//    glm::vec4 controlPointE = glm::vec4(90, 7, -126, 1);
//    glm::vec4 controlPointF = glm::vec4(100, 7, -80, 1);
//    glm::vec4 controlPointG = glm::vec4(120, 7, -26, 1);
//    glm::vec4 controlPointH = glm::vec4(140, 7, -16, 1);

    glm::vec4 controlPointA = glm::vec4(-3.12691, 32, 20, 1);
    glm::vec4 controlPointB = glm::vec4(15, 5, -20, 1);
    glm::vec4 controlPointC = glm::vec4(30, 8, -40, 1);
    glm::vec4 controlPointD = glm::vec4(45, 8, -60, 1);
    glm::vec4 controlPointE = glm::vec4(75, 8, -75, 1);
    glm::vec4 controlPointF = glm::vec4(100, 12, -90, 1);
    glm::vec4 controlPointG = glm::vec4(120, 9, -110, 1);
    glm::vec4 controlPointH = glm::vec4(140, 7, -140, 1);

//    glm::vec4 controlPointA = glm::vec4(-3.12691, 32, 20, 1);
//    glm::vec4 controlPointB = glm::vec4(-0.04, 5, -10, 1);
//    glm::vec4 controlPointC = glm::vec4(10, 8, -20, 1);
//    glm::vec4 controlPointD = glm::vec4(19, 7, -58, 1);
//    glm::vec4 controlPointE = glm::vec4(33, 6, -108, 1);
//    glm::vec4 controlPointF = glm::vec4(59, 6, -146, 1);
//    glm::vec4 controlPointG = glm::vec4(50, 4, -180, 1);
//    glm::vec4 controlPointH = glm::vec4(32, 7, -154, 1);

    std::vector<glm::vec4> controlPoints;
    controlPoints.push_back(controlPointA);
    controlPoints.push_back(controlPointB);
    controlPoints.push_back(controlPointC);
    controlPoints.push_back(controlPointD);
    controlPoints.push_back(controlPointE);
    controlPoints.push_back(controlPointF);
    controlPoints.push_back(controlPointG);
    controlPoints.push_back(controlPointH);

    std::vector<glm::vec4> pathPoints;

    for (int i = 0; i < 2000; i++) {
        float t = static_cast<float>(i)/1999.0f;
        // float t = pow(static_cast<float>(i) / 499.0f, 2.0f);
        glm::vec4 point;
        Bezier::findCurvePoint(point, controlPoints, t);
//        Bezier::findCurvePoint(p, controlPointA, controlPointB, controlPointC, controlPointD, t);

//        std::cout << "Point x:  " << point.x << std::endl;
//        std::cout << "pathPoint.x: " << point.x << " pathPoint.y: " << point.y << " pathPoint.z: " << point.z << std::endl;

        pathPoints.push_back(glm::vec4(point));

        // if you know the tangent vector for the previous point and you calculate the tangent for the current point;
        // the more the dot product between those two the more you can rotate the camera

        // fixed downward path for the look vector
        // another thing is try to make the look vector tangential to the curve
    }

//    std::cout << "*** JUMP ****" << std::endl;

//    glm::vec3 controlPointA1 = glm::vec3(75, 7, -126);
//    glm::vec3 controlPointB1 = glm::vec3(80, 5, -120);
//    glm::vec3 controlPointC1 = glm::vec3(95, 8, -100);
//    glm::vec3 controlPointD1 = glm::vec3(110, 3, -148);


//    for (int i = 0; i < 500; i++) {
//        float t = static_cast<float>(i)/499.0f;
//        // float t = pow(static_cast<float>(i) / 499.0f, 2.0f);
//        glm::vec3 p;
//        Bezier::findCurvePoint(p, controlPointA1, controlPointB1, controlPointC1, controlPointD1, t);
//        std::cout << "pathPoint.x: " << p.x << " pathPoint.y: " << p.y << " pathPoint.z: " << p.z << std::endl;

//        pathPoints.push_back(glm::vec4(p, 1));

        // if you know the tangent vector for the previous point and you calculate the tangent for the current point;
        // the more the dot product between those two the more you can rotate the camera

        // fixed downward path for the look vector
        // another thing is try to make the look vector tangential to the curve
//    }

//    glm::vec3 controlPointA2 = glm::vec3(-3.12691, 32, 20);
//    glm::vec3 controlPointB2 = glm::vec3(15, 5, -20);
//    glm::vec3 controlPointC2 = glm::vec3(45, 8, -80);
//    glm::vec3 controlPointD2 = glm::vec3(75, 7, -126);


//    for (int i = 0; i < 500; i++) {
//        float t = static_cast<float>(i)/499.0f;
//        // float t = pow(static_cast<float>(i) / 499.0f, 2.0f);
//        glm::vec3 p;
//        Bezier::findCurvePoint(p, controlPointA2, controlPointB2, controlPointC2, controlPointD2, t);
//        std::cout << "pathPoint.x: " << p.x << " pathPoint.y: " << p.y << " pathPoint.z: " << p.z << std::endl;

//        pathPoints.push_back(glm::vec4(p, 1));

//        // if you know the tangent vector for the previous point and you calculate the tangent for the current point;
//        // the more the dot product between those two the more you can rotate the camera

//        // fixed downward path for the look vector
//        // another thing is try to make the look vector tangential to the curve
//    }

//    glm::vec3 controlPointA3 = glm::vec3(-3.12691, 32, 20);
//    glm::vec3 controlPointB3 = glm::vec3(15, 5, -20);
//    glm::vec3 controlPointC3 = glm::vec3(45, 8, -80);
//    glm::vec3 controlPointD3 = glm::vec3(75, 7, -126);


//    for (int i = 0; i < 500; i++) {
//        float t = static_cast<float>(i)/499.0f;
//        // float t = pow(static_cast<float>(i) / 499.0f, 2.0f);
//        glm::vec3 p;
//        Bezier::findCurvePoint(p, controlPointA3, controlPointB3, controlPointC3, controlPointD3, t);
//        std::cout << "pathPoint.x: " << p.x << " pathPoint.y: " << p.y << " pathPoint.z: " << p.z << std::endl;

//        pathPoints.push_back(glm::vec4(p, 1));

//        // if you know the tangent vector for the previous point and you calculate the tangent for the current point;
//        // the more the dot product between those two the more you can rotate the camera

//        // fixed downward path for the look vector
//        // another thing is try to make the look vector tangential to the curve
//    }

    return pathPoints;
}
