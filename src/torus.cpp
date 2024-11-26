#include "torus.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <cmath>

void generateTorusVerts(std::vector<glm::vec3> &verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
    float outerRadius, float innerRadius, int sides, int rings)
{
    constexpr float TWOPI = 2 * glm::pi<float>();
    int faces = sides * rings;
    float ringFactor = (float)(TWOPI / rings);
    float sideFactor = (float)(TWOPI / sides);
    int tidx = 0;
    for (int ring = 0; ring <= rings; ring++) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);
        for (int side = 0; side < sides; side++) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);
            float r = (outerRadius + innerRadius * cv);
            verts.push_back(glm::vec3({ r * cu, r * su, innerRadius * sv }));
            norms.push_back(glm::normalize(glm::vec3({ cv * cu * r, cv * su * r, sv * r })));

            //             tex[tidx] = (float)(u / TWOPI);
            //             tex[tidx+1] = (float)(v / TWOPI);
            //            tidx += 2;
                        // Normalize
        }
    }

    for (int ring = 0; ring < rings; ring++) {
        int ringStart = ring * sides;
        int nextRingStart = (ring + 1) * sides;
        for (int side = 0; side < sides; side++) {
            int nextSide = (side + 1) % sides;
            // The quad
            elements.push_back(ringStart + side);
            elements.push_back(nextRingStart + side);
            elements.push_back(nextRingStart + nextSide);
            elements.push_back(ringStart + side);
            elements.push_back(nextRingStart + nextSide);
            elements.push_back(ringStart + nextSide);
        }
    }
}