#include "shapes.hpp"
#include "teapotData.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <cmath>
#include <iostream>


glm::vec3 evaluate(int gridU, int gridV, float* B, glm::vec3 patch[][4])
{
    glm::vec3 p(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
        }
    }
    return p;
}

glm::vec3 evaluateNormal(int gridU, int gridV, float* B, float* dB, glm::vec3 patch[][4])
{
    glm::vec3 du(0.0f, 0.0f, 0.0f);
    glm::vec3 dv(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
            dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
        }
    }
    return glm::normalize(glm::cross(du, dv));
}


void computeBasisFunctions(float* B, float* dB, int grid) {
    float inc = 1.0f / grid;
    for (int i = 0; i <= grid; i++)
    {
        float t = i * inc;
        float tSqr = t * t;
        float oneMinusT = (1.0f - t);
        float oneMinusT2 = oneMinusT * oneMinusT;

        B[i * 4 + 0] = oneMinusT * oneMinusT2;
        B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
        B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
        B[i * 4 + 3] = t * tSqr;

        dB[i * 4 + 0] = -3.0f * oneMinusT2;
        dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
        dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
        dB[i * 4 + 3] = 3.0f * tSqr;
    }
}

void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV)
{
    for (int u = 0; u < 4; u++) {          // Loop in u direction
        for (int v = 0; v < 4; v++) {     // Loop in v direction
            if (reverseV) {
                patch[u][v] = glm::vec3(
                    Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][0],
                    Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][1],
                    Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][2]
                );
            }
            else {
                patch[u][v] = glm::vec3(
                    Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][0],
                    Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][1],
                    Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][2]
                );
            }
        }
    }
}

void buildPatch(glm::vec3 patch[][4],
    float* B, float* dB,
    float* v, float* n, float* tc,
    unsigned int* el,
    int& index, int& elIndex, int& tcIndex, int grid, glm::mat3 reflect,
    bool invertNormal)
{
    int startIndex = index / 3;
    float tcFactor = 1.0f / grid;

    for (int i = 0; i <= grid; i++)
    {
        for (int j = 0; j <= grid; j++)
        {
            glm::vec3 pt = reflect * evaluate(i, j, B, patch);
            glm::vec3 norm = reflect * evaluateNormal(i, j, B, dB, patch);
            if (invertNormal)
                norm = -norm;

            v[index] = pt.x;
            v[index + 1] = pt.y;
            v[index + 2] = pt.z;

            n[index] = norm.x;
            n[index + 1] = norm.y;
            n[index + 2] = norm.z;

            tc[tcIndex] = i * tcFactor;
            tc[tcIndex + 1] = j * tcFactor;

            index += 3;
            tcIndex += 2;
        }
    }

    for (int i = 0; i < grid; i++)
    {
        int iStart = i * (grid + 1) + startIndex;
        int nextiStart = (i + 1) * (grid + 1) + startIndex;
        for (int j = 0; j < grid; j++)
        {
            el[elIndex] = iStart + j;
            el[elIndex + 1] = nextiStart + j + 1;
            el[elIndex + 2] = nextiStart + j;

            el[elIndex + 3] = iStart + j;
            el[elIndex + 4] = iStart + j + 1;
            el[elIndex + 5] = nextiStart + j + 1;

            elIndex += 6;
        }
    }
}

void buildPatchReflect(int patchNum,
    float* B, float* dB,
    float* v, float* n,
    float* tc, unsigned int* el,
    int& index, int& elIndex, int& tcIndex, int grid,
    bool reflectX, bool reflectY)
{
    glm::vec3 patch[4][4];
    glm::vec3 patchRevV[4][4];
    getPatch(patchNum, patch, false);
    getPatch(patchNum, patchRevV, true);

    // Patch without modification
    buildPatch(patch, B, dB, v, n, tc, el,
        index, elIndex, tcIndex, grid, glm::mat3(1.0f), true);

    // Patch reflected in x
    if (reflectX) {
        buildPatch(patchRevV, B, dB, v, n, tc, el,
            index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)), false);
    }

    // Patch reflected in y
    if (reflectY) {
        buildPatch(patchRevV, B, dB, v, n, tc, el,
            index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)), false);
    }

    // Patch reflected in x and y
    if (reflectX && reflectY) {
        buildPatch(patch, B, dB, v, n, tc, el,
            index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)), true);
    }
}

void generatePatches(float* v, float* n, float* tc, unsigned int* el, int grid) {
    float* B = new float[4 * (grid + 1)];  // Pre-computed Bernstein basis functions
    float* dB = new float[4 * (grid + 1)]; // Pre-computed derivitives of basis functions

    int idx = 0, elIndex = 0, tcIndex = 0;

    // Pre-compute the basis functions  (Bernstein polynomials)
    // and their derivatives
    computeBasisFunctions(B, dB, grid);

    // Build each patch
    // The rim
    buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The body
    buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The lid
    buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The bottom
    buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The handle
    buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
    buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
    // The spout
    buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
    buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

    delete[] B;
    delete[] dB;
}

void moveLid(int grid, float* v, glm::mat4 lidTransform) {

    int start = 3 * 12 * (grid + 1) * (grid + 1);
    int end = 3 * 20 * (grid + 1) * (grid + 1);

    for (int i = start; i < end; i += 3)
    {
        glm::vec4 vert = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
        vert = lidTransform * vert;
        v[i] = vert.x;
        v[i + 1] = vert.y;
        v[i + 2] = vert.z;
    }
}

void generateTeapot(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements, 
    int grid, glm::mat4 lidTransform) {
    uint32_t nverts = 32 * (grid + 1) * (grid + 1);
    uint32_t faces = grid * grid * 32;

    float* v = new float[nverts * 3];  //vertex positions : vec3
    float* n = new float[nverts * 3];  //vertex normals : vec3
    float* tc = new float[nverts * 2]; //texture coordinates : vec2 (we don't use it at this point)
    unsigned int* el = new unsigned int[faces * 6];  //indices for IBO 

    generatePatches(v, n, tc, el, grid);

    verts.reserve(nverts * 3);
    norms.reserve(nverts * 3);

    for (uint32_t i = 0; i < nverts * 3; i += 3) {
        verts.push_back({ v[i], v[i + 1], v[i + 2] });
        norms.push_back({ n[i], n[i + 1], n[i + 2] });
    }

    elements.reserve(faces * 6);
    copy(&el[0], &el[faces * 6], back_inserter(elements));

    delete[] v;
    delete[] n;
    delete[] el;
    delete[] tc;
}

void generateSphere(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements, 
	float radius, uint32_t slices, uint32_t stacks)
{
	// Generate positions and normals
	float theta, phi;
	float thetaFac = glm::two_pi<float>() / slices;
	float phiFac = glm::pi<float>() / stacks;
	float nx, ny, nz, s, t;
	uint32_t nVerts = (slices + 1) * (stacks + 1);
	uint32_t elementsLen = (slices * 2 * (stacks - 1)) * 3;
	elements.reserve(elementsLen);
	verts.reserve(nVerts);
	norms.reserve(nVerts);

	//uint32_t tIdx = 0;
	//float* tex = new float[2 * nVerts];    //we don't use it now
	for (uint32_t i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (float)i / slices;
		for (uint32_t j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (float)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts.push_back({ radius * nx, radius * ny, radius * nz });
			norms.push_back({ nx, ny, nz });
			//tex[tIdx] = s;
			//tex[tIdx + 1] = t;
			//tIdx += 2;
		}
	}

	// Generate the element list
	for (uint32_t i = 0; i < slices; i++) {
		uint32_t stackStart = i * (stacks + 1);
		uint32_t nextStackStart = (i + 1) * (stacks + 1);
		for (uint32_t j = 0; j < stacks; j++) {
			if (j == 0) {
				elements.push_back(stackStart);
				elements.push_back(stackStart + 1);
				elements.push_back(nextStackStart + 1);
			}
			else if (j == stacks - 1) {
				elements.push_back(stackStart + j);
				elements.push_back(stackStart + j + 1);
				elements.push_back(nextStackStart + j);
			}
			else {
				elements.push_back(stackStart + j);
				elements.push_back(stackStart + j + 1);
				elements.push_back(nextStackStart + j + 1);
				elements.push_back(nextStackStart + j);
				elements.push_back(stackStart + j);
				elements.push_back(nextStackStart + j + 1);
			}
		}
	}
}

void generateCube(std::vector<glm::vec3>& verts, std::vector<uint32_t>& elements, float size) {
    size /= 2;

    verts = {
        {-size, -size, -size},
        {-size, -size, +size},
        {-size, +size, +size},
        {-size, +size, -size},

        {+size, +size, +size},
        {+size, -size, +size},
        {+size, -size, -size},
        {+size, +size, -size},

        {-size, -size, -size},
        {+size, -size, -size},
        {+size, -size, +size},
        {-size, -size, +size},

        {+size, +size, +size},
        {+size, +size, -size},
        {-size, +size, -size},
        {-size, +size, +size},

        {-size, -size, -size},
        {-size, +size, -size},
        {+size, +size, -size},
        {+size, -size, -size},

        {+size, +size, +size},
        {-size, +size, +size},
        {-size, -size, +size},
        {+size, -size, +size},
    };

    elements = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
    };
}

void generatePlane(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
    std::vector<glm::vec3>& color, int resolution, float size) {
    float tile_size = size / resolution;

    for (int i = 0; i != resolution; i++) {
        for (int j = 0; j != resolution; j++) {
            //std::cout << i  << "   " << (i - (i % 2 == 0)) << "   " << (i - (i % 2 == 0)) * tile_size << "   " << -size / 2 << "   " << (i - (i % 2 == 0)) * tile_size - size / 2 << "   " << (j - (j % 2 == 0)) * tile_size - size / 2 << std::endl;
            
            verts.push_back(glm::vec3(i * tile_size - size / 2, 0., j * tile_size - size / 2));
            verts.push_back(glm::vec3((i + 1) * tile_size - size / 2, 0., j * tile_size - size / 2));
            verts.push_back(glm::vec3(i * tile_size - size / 2, 0., (j + 1) * tile_size - size / 2));
            verts.push_back(glm::vec3((i + 1) * tile_size - size / 2, 0., (j + 1) * tile_size - size / 2));

            for (int h = 0; h != 4; h++) {
                norms.push_back(glm::vec3(0., 1., 0.));
                if ((i % 2 == 0) ^ (j % 2 == 0)) {
                    color.push_back(glm::vec3(0.1, 0.1, 0.1));
                } else {
                    color.push_back(glm::vec3(0.7, 0.7, 0.7));
                }
            }

            elements.push_back(i * resolution * 4 + j * 4 + 1);
            elements.push_back(i * resolution * 4 + j * 4);
            elements.push_back(i * resolution * 4 + j * 4 + 2);
            elements.push_back(i * resolution * 4 + j * 4 + 1);
            elements.push_back(i * resolution * 4 + j * 4 + 2);
            elements.push_back(i * resolution * 4 + j * 4 + 3);
        }
    }
}

void generateTorus(std::vector<glm::vec3> &verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
    float outerRadius, float innerRadius, int sides, int rings)
{
    constexpr float TWOPI = 2 * glm::pi<float>();
    int faces = sides * rings;
    float ringFactor = (float)(TWOPI / rings);
    float sideFactor = (float)(TWOPI / sides);
    //int tidx = 0;
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