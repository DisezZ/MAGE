#pragma once

#include "slx.hpp"

#include "NoCopy.hpp"
#include "MageDevice.hpp"

namespace mage
{
    struct PipelineConfigInfo
    {
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
        VkShaderModule fragShaderModuel;

    public:
        MagePipeline(MageDevice &device, const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo &configInfo);
        ~MagePipeline();

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    };
} // namespace mage