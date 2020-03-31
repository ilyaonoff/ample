#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <cmath>
#include <vector>

#include "Vector3d.h"
#include "Vector2d.h"
#include "VertexArray.h"
#include "GraphicalEdge.h"
#include "Debug.h"

namespace ample::graphics
{
static std::vector<Vector3d<float>> generateSideCoords(const std::vector<Vector2d<float>> &graphicalShape,
                                                       const float z,
                                                       const float depth)
{
    std::vector<Vector3d<float>> sideArray;
    size_t verts = graphicalShape.size();
    for (size_t vId = 0; vId < verts; ++vId)
    {
        int nvt = (vId + 1) % verts;
        sideArray.emplace_back(graphicalShape[vId].x, graphicalShape[vId].y, z);
        sideArray.emplace_back(graphicalShape[vId].x, graphicalShape[vId].y, depth + z);
        sideArray.emplace_back(graphicalShape[nvt].x, graphicalShape[nvt].y, depth + z);

        sideArray.emplace_back(graphicalShape[vId].x, graphicalShape[vId].y, z);
        sideArray.emplace_back(graphicalShape[nvt].x, graphicalShape[nvt].y, depth + z);
        sideArray.emplace_back(graphicalShape[nvt].x, graphicalShape[nvt].y, z);
    }
    return sideArray;
}

static std::vector<Vector3d<float>> generateSideNormals(const std::vector<Vector2d<float>> &graphicalShape,
                                                        normalsMode mode,
                                                        const float z,
                                                        const float depth)
{
    auto vert = generateSideCoords(graphicalShape, z, depth);
    ASSERT(vert.size() % 3 == 0);
    size_t verts = vert.size();
    std::vector<Vector3d<float>> normals;
    std::vector<glm::vec3> triangleNormals;
    std::vector<int> normId;
    for (size_t i = 0; i < verts; i += 3)
    {
        glm::vec3 first{vert[i + 0].x, vert[i + 0].y, vert[i + 0].z};
        glm::vec3 secon{vert[i + 1].x, vert[i + 1].y, vert[i + 1].z};
        glm::vec3 third{vert[i + 2].x, vert[i + 2].y, vert[i + 2].z};
        auto norm = glm::triangleNormal(std::move(first),
                                        std::move(secon),
                                        std::move(third));
        triangleNormals.emplace_back(norm);
        triangleNormals.emplace_back(norm);
        triangleNormals.emplace_back(norm);
        for (int i = 0; i < 3; ++i)
        {
            normals.emplace_back(norm.x, norm.y, norm.z);
        }
    }
    if (mode == normalsMode::VERTEX)
    {
        auto normalsCopy{triangleNormals};
        for (size_t i = 2; i < verts; i += 6)
        {
            size_t f = (i + 2) % verts;
            size_t s = (i + 5) % verts;
            auto mid = (normalsCopy[i] + normalsCopy[f] + normalsCopy[s]) * (1.0f / 3.0f);
            normals[i] = normals[f] = normals[s] = {mid.x, mid.y, mid.z};
        }
        for (size_t i = 5; i < verts; i += 6)
        {
            size_t f = (i + 1) % verts;
            size_t s = (i + 4) % verts;
            auto mid = (normalsCopy[i] + normalsCopy[f] + normalsCopy[s]) * (1.0f / 3.0f);
            normals[i] = normals[f] = normals[s] = {mid.x, mid.y, mid.z};
        }
    }
    return normals;
}

static std::vector<Vector2d<float>> generateSideUVCoords(const std::vector<Vector2d<float>> &graphicalShape,
                                                         const float z,
                                                         const float depth,
                                                         const Vector2d<float> &repeats)
{
    auto shape = generateSideCoords(graphicalShape, z, depth);
    std::vector<Vector2d<float>> uvCoords(shape.size());

    float boardLength = std::sqrt((graphicalShape[0].x - graphicalShape.back().x) * (graphicalShape[0].x - graphicalShape.back().x) +
                                  (graphicalShape[0].y - graphicalShape.back().y) * (graphicalShape[0].y - graphicalShape.back().y));
    std::vector<float> prefixBoardLenth(shape.size() + 1);
    for (size_t i = 1; i < graphicalShape.size(); ++i)
    {
        boardLength += std::sqrt((graphicalShape[i].x - graphicalShape[i - 1].x) * (graphicalShape[i].x - graphicalShape[i - 1].x) +
                                 (graphicalShape[i].y - graphicalShape[i - 1].y) * (graphicalShape[i].y - graphicalShape[i - 1].y));
        prefixBoardLenth[i] = boardLength;
    }
    for (size_t i = 0, vId = 0; i < uvCoords.size(); i += 6, vId += 1)
    {
        uvCoords[i + 0].x = uvCoords[i + 3].x = uvCoords[i + 5].x = 0.f;
        uvCoords[i + 1].x = uvCoords[i + 2].x = uvCoords[i + 4].x = repeats.x;
        uvCoords[i + 0].y = uvCoords[i + 1].y = uvCoords[i + 3].y = prefixBoardLenth[vId + 0] / boardLength * repeats.y;
        uvCoords[i + 2].y = uvCoords[i + 4].y = uvCoords[i + 5].y = prefixBoardLenth[vId + 1] / boardLength * repeats.y;
    }

    return uvCoords;
}

GraphicalEdge::GraphicalEdge(const std::vector<Vector2d<float>> &shape,
                             const float z,
                             const float thickness,
                             const Vector2d<float> &textureRepeats,
                             const normalsMode normMode,
                             const glm::mat4 &translated,
                             const glm::mat4 &scaled,
                             const glm::mat4 &rotated)
    : GraphicalObject(translated, scaled, rotated)
{
    bindVertexArray(std::make_shared<VertexArray>(generateSideCoords(shape, z, thickness),
                                                  generateSideUVCoords(shape, z, thickness, textureRepeats),
                                                  generateSideNormals(shape, normMode, z, thickness)));
}

} // namespace ample::graphics