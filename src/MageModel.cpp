#include "MageModel.hpp"

namespace mage
{
    MageModel::MageModel(MageDevice &device, const std::vector<Vertex> &vertices) : mageDevice{device}
    {
        createVertexBuffers(vertices);
    }

    MageModel::~MageModel()
    {
        vkDestroyBuffer(mageDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(mageDevice.device(), vertexBufferMemory, nullptr);
    }

    void MageModel::createVertexBuffers(const std::vector<Vertex> &vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "vertex count must be greater than 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        mageDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory);
        
        void *data;
        vkMapMemory(mageDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(mageDevice.device(), vertexBufferMemory);
    }

    void MageModel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffer[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffer, offsets);
    }
    
    void MageModel::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    std::vector<VkVertexInputBindingDescription> MageModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> MageModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = 0;

        return attributeDescriptions;
    }
} // namespace mage
