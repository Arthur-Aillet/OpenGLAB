
#define _USE_MATH_DEFINES 
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <chrono>

#include "cow.h"
#include "ppm_io.h"


glm::vec3 convertNDCtoImage(const glm::vec4 vertexNDC, glm::vec4 vertexView, const uint32_t& imageWidth, const uint32_t& imageHeight)
{
    return { 
        (vertexNDC.x + 1) / 2 * imageWidth,
        (1 - vertexNDC.y) / 2 * imageHeight,
        -vertexView.z
    };
}

float min3(const float& a, const float& b, const float& c)
{
    return std::min(a, std::min(b, c));
}

float max3(const float& a, const float& b, const float& c)
{
    return std::max(a, std::max(b, c));
}

float edge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
    glm::vec3 zc = glm::normalize(pos - look);
    glm::vec3 xc = glm::normalize(glm::cross(up, zc));
    glm::vec3 yc = glm::normalize(glm::cross(zc, xc));

    return { 
        xc.x, yc.x, zc.x, 0,
        xc.y, yc.y, zc.y, 0,
        xc.z, yc.z, zc.z, 0,
        -glm::dot(pos, xc), -glm::dot(pos, yc), -glm::dot(pos, zc), 1
    };
}

glm::mat4 perspective(float fovy, float aspect, float near, float far)
{
    return {
        1 / (aspect * std::tan(glm::radians(fovy) / 2)), 0, 0, 0,
        0, 1 / std::tan(glm::radians(fovy) / 2), 0, 0,
        0, 0, -(far + near) / (far - near), -1,
        0, 0, -2.0f * (far * near / (far - near)), 0,
    };
}

const uint32_t imageWidth = 640;
const uint32_t imageHeight = 480;


const uint32_t ntris = 3156;
const float nearClippingPlane = 1;
const float farClippingPLane = 1000;

int main(int argc, char** argv)
{
    std::cout << "Started!" << std::endl;

    PPM ppmOut;
    ppmOut.setBinary(true);

    struct rgb
    {
        uint8_t r, g, b;
    };

    std::vector<rgb>  frameBuffer;
    frameBuffer.resize(imageWidth * imageHeight);


    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        frameBuffer[i].r = 0;
        frameBuffer[i].g = 0;
        frameBuffer[i].b = 0;
    }

    float* depthBuffer = new float[imageWidth * imageHeight];

    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        depthBuffer[i] = farClippingPLane;
    }

    auto t_start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < ntris; ++i) {

        const glm::vec3& v0 = vertices[nvertices[i * 3]];
        const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
        const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];


        glm::mat4 modelMatrix(1.0f);

        glm::mat4 viewMatrix = lookAt(glm::vec3(10, -20, 0), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        //Camera(view) coordinates
        glm::vec4 v0e = modelViewMatrix * glm::vec4(v0, 1);
        glm::vec4 v1e = modelViewMatrix * glm::vec4(v1, 1);
        glm::vec4 v2e = modelViewMatrix * glm::vec4(v2, 1);

        glm::mat4 projectionMatrix = perspective(45.0f, imageWidth / (float)imageHeight, nearClippingPlane, farClippingPLane);

        //Clip coodinates
        glm::vec4 v0c = projectionMatrix * v0e;
        glm::vec4 v1c = projectionMatrix * v1e;
        glm::vec4 v2c = projectionMatrix * v2e;

        //Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w   (same to v1c, v2c)
        v0c = { v0c.x / v0c.w, v0c.y / v0c.w, v0c.z / v0c.w, v0c.w };
        v1c = { v1c.x / v1c.w, v1c.y / v1c.w, v1c.z / v1c.w, v1c.w };
        v2c = { v2c.x / v2c.w, v2c.y / v2c.w, v2c.z / v2c.w, v2c.w };

        glm::vec3 v0Raster = convertNDCtoImage(v0c, v0e, imageWidth, imageHeight);
        glm::vec3 v1Raster = convertNDCtoImage(v1c, v1e, imageWidth, imageHeight);
        glm::vec3 v2Raster = convertNDCtoImage(v2c, v2e, imageWidth, imageHeight);

        float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y);
        float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y);

        if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

        uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
        uint32_t x1 = std::min(int32_t(imageWidth) - 1, (int32_t)(std::floor(xmax)));
        uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
        uint32_t y1 = std::min(int32_t(imageHeight) - 1, (int32_t)(std::floor(ymax)));


        //calculat the area of triangle  (area)
        float area = edge(v0Raster, v1Raster, v2Raster);

        for (uint32_t y = y0; y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {

                glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);

                //calculate the areas of  three suvdivided triangles
                float w0 = edge(v1Raster, v2Raster, pixelSample); //w0
                float w1 = edge(v2Raster, v0Raster, pixelSample); //w1
                float w2 = edge(v0Raster, v1Raster, pixelSample); //w2


                if (w0 >= 0 && w1 >= 0 && w2 >= 0) { //inside
                    //calculate the  ratio here
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    //calculate the z of pixelSample
                    float oneOverZ = w0 / v0Raster.z  + w1 / v1Raster.z + w2 / v2Raster.z;
                    float z = 1 / oneOverZ;

                    if (z < depthBuffer[y * imageWidth + x]) {
                        depthBuffer[y * imageWidth + x] = z;

                        //calculate normal vector from v0e, v1e, v2e
                        glm::vec3 v10e = v1e - v0e;
                        glm::vec3 v12e = v2e - v1e;

                        glm::vec3 n = (glm::normalize(glm::cross(v10e, v12e)) + glm::vec3(1, 1, 1)) / 2.0f;

                        frameBuffer[y * imageWidth + x].r = n.x * 255;
                        frameBuffer[y * imageWidth + x].g = n.y * 255;
                        frameBuffer[y * imageWidth + x].b = n.z * 255;
                    }
                }
            }
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    auto passedTime = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cerr << "Wall passed time: " << passedTime << "ms" << std::endl;


    rgb* p = frameBuffer.data();

    ppmOut.load(&p[0].r, imageHeight, imageWidth, 255, "P6");
    ppmOut.write("../../../../output.ppm");

    delete[] depthBuffer;

    return 0;
}