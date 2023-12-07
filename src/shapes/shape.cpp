#include "shape.h"

Shape::Shape()
{

}

glm::vec3 Shape::getTang(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
    glm::vec3 Edge1 = v1 - v0;
    glm::vec3 Edge2 = v2 - v0;

    float DeltaU1 = v1[0] - v0[0];
    float DeltaV1 = v1[2] - v0[2];
    float DeltaU2 = v2[0] - v0[0];
    float DeltaV2 = v2[2] - v0[2];

    float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

    glm::vec3 Tangent, Bitangent;

    Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
    Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
    Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

    Bitangent.x = f * (-DeltaU2 * Edge1.x + DeltaU1 * Edge2.x);
    Bitangent.y = f * (-DeltaU2 * Edge1.y + DeltaU1 * Edge2.y);
    Bitangent.z = f * (-DeltaU2 * Edge1.z + DeltaU1 * Edge2.z);

    return glm::normalize(Tangent);
}
