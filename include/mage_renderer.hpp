#pragma once

#include "slx.hpp"
#include "glx.hpp"

#include "no_copy.hpp"
#include "mage_window.hpp"
#include "mage_device.hpp"
#include "mage_swap_chain.hpp"
#include "mage_model.hpp"

namespace mage
{
    class MageRenderer final : NoCopy
    {
    public:

        MageRenderer(MageWindow &window, MageDevice &device);
        ~MageRenderer();

        VkRenderPass getSwapChainRenderPass() const { return mageSwapChain->getRenderPass(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Can't get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Can't get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();

        MageWindow &mageWindow;
        MageDevice &mageDevice;
        std::unique_ptr<MageSwapChain> mageSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
} // namespace mage
