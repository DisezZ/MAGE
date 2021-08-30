#pragma once

#include "slx.hpp"
#include "glx.hpp"

#include "NoCopy.hpp"
#include "MageWindow.hpp"
#include "MagePipeline.hpp"
#include "MageDevice.hpp"
#include "MageSwapChain.hpp"
#include "MageModel.hpp"

namespace mage
{
    class FirstApp final : NoCopy
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        void run();

    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        MageWindow mageWindow{WIDTH, HEIGHT, "Vulkan::Mage Engine"};
        MageDevice mageDevice{mageWindow};
        MageSwapChain mageSwapChain{mageDevice, mageWindow.getExtent()};
        std::unique_ptr<MagePipeline> magePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<MageModel> mageModel;
    };
} // namespace mage
