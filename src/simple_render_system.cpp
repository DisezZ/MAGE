#include <slx.hpp>

#include "simple_render_system.hpp"

namespace mage
{
    struct PushConstantData {
        alignas(16) glm::mat4 transform;
        alignas(16) glm::vec3 color;
    };

    SimpleRenderSystem::SimpleRenderSystem(MageDevice &device, VkRenderPass renderPass) : mageDevice{device} 
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        vkDestroyPipelineLayout(mageDevice.device(), pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(mageDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "can't create pipeline before pipeline layout");
        PipelineConfigInfo pipelineConfig{};
        MagePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        magePipeline = std::make_unique<MagePipeline>(
            mageDevice,
            "shaders/shaderSimple.vert.spv",
            "shaders/shaderSimple.frag.spv",
            pipelineConfig);
    }

    void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MageGameObject> &gameObjects, const MageCamera &camera) 
    {
        magePipeline->bind(commandBuffer);

        for(auto &obj : gameObjects)
        {
            obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.002f, glm::two_pi<float>());
            obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.001f, glm::two_pi<float>());

            PushConstantData push{};
            push.color = obj.color;
            push.transform = camera.getProjection() * obj.transform.mat4();

            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }


} // namespace mage
