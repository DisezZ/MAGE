#pragma once

#include "slx.hpp"

#include "NoCopy.hpp"
#include "MageDevice.hpp"
#include "MageModel.hpp"

namespace mage
{
    struct PipelineConfigInfo
    {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class MagePipeline final : NoCopy
    {
    private:
        static std::vector<char> readFile(const std::string &filePath);
        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        MageDevice &mageDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

    public:
        MagePipeline(MageDevice &device, const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
        ~MagePipeline();

        void bind(VkCommandBuffer commandBuffer);
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    };
} // namespace mage