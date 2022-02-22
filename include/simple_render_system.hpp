#pragma once

#include "slx.hpp"
#include "glx.hpp"

#include "no_copy.hpp"
#include "mage_pipeline.hpp"
#include "mage_device.hpp"
#include "mage_game_object.hpp"
#include "mage_camera.hpp"

namespace mage
{
    class SimpleRenderSystem final : NoCopy
    {
    public:
        SimpleRenderSystem(MageDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MageGameObject> &gameObjects, const MageCamera &camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        MageDevice &mageDevice;

        std::unique_ptr<MagePipeline> magePipeline;
        VkPipelineLayout pipelineLayout;
    };
} // namespace mage
