#pragma once

#include "slx.hpp"
#include "glx.hpp"

#include "NoCopy.hpp"
#include "MageDevice.hpp"

namespace mage
{
    class MageModel : NoCopy
    {
    public:
        struct Vertex
        {
            glm::vec3 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        MageModel(MageDevice &device, const std::vector<Vertex> &vertices);
        ~MageModel();

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        MageDevice &mageDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
} // namespace mage
