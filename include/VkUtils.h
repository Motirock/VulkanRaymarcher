#ifndef VK_UTILS
#define VK_UTILS

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <cmath>
#include <functional>

namespace VkUtils {
    enum Orientation {POSITIVE_X, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z, NUMBER_OF_ORIENTATIONS};

    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }

        bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color;
        }
    };

    const int VOXEL_GRID_SIZE = 8;
    struct UniformBufferObject {
        alignas(16) glm::vec3 position;   
        alignas(16) glm::vec3 viewDirection;
        alignas(16) float FOV;
        // alignas(16) const uint32_t CHILDREN_INDEX_MASK  =   0b00000000011111111111111111111111;
        // alignas(16) const uint32_t IS_HOMOGENEOUS_MASK  =   0b00000000100000000000000000000000;
        // alignas(16) const uint32_t CHILDREN_MASK_MASK   =   0b11111111000000000000000000000000;
        // alignas(16) const uint32_t COLOR_RED_MASK       =   0b00000000000000000000000011111111;
        // alignas(16) const uint32_t COLOR_GREEN_MASK     =   0b00000000000000001111111100000000;
        // alignas(16) const uint32_t COLOR_BLUE_MASK      =   0b00000000111111110000000000000000;
        // alignas(16) const uint32_t COLOR_ALPHA_MASK     =   0b11111111000000000000000000000000;
        // alignas(16) const uint32_t CHILDREN_MASK_MASKS[8] = {
        //                                     0b00000001000000000000000000000000,
        //                                     0b00000010000000000000000000000000,
        //                                     0b00000100000000000000000000000000,
        //                                     0b00001000000000000000000000000000,
        //                                     0b00010000000000000000000000000000,
        //                                     0b00100000000000000000000000000000,
        //                                     0b01000000000000000000000000000000,
        //                                     0b10000000000000000000000000000000;
        // };
    };
}

#endif