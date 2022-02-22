#include <slx.hpp>

#include "mage_renderer.hpp"

namespace mage
{
    struct PushConstantData {
        alignas(16) glm::mat4 transform;
        //alignas(16) glm::vec3 offset;
        //alignas(16) glm::vec3 color;
    };

    MageRenderer::MageRenderer(MageWindow &window, MageDevice &device) : mageWindow{window}, mageDevice{device} {
        recreateSwapChain();
        createCommandBuffers();
    }

    MageRenderer::~MageRenderer()
    {
        freeCommandBuffers();
    }

    void MageRenderer::recreateSwapChain()
    {
        auto extent = mageWindow.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = mageWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(mageDevice.device());

        if (mageSwapChain == nullptr)
            mageSwapChain = std::make_unique<MageSwapChain>(mageDevice, extent);
        else
        {
            std::shared_ptr<MageSwapChain> oldSwapChain = std::move(mageSwapChain);
            mageSwapChain = std::make_unique<MageSwapChain>(mageDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*mageSwapChain.get()))
            {
                throw std::runtime_error("Swap chain image(or depth) format has changed");
            }
        }
        // we'll come back to this later
    }

    void MageRenderer::createCommandBuffers()
    {
        commandBuffers.resize(MageSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = mageDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(mageDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("falied to allocate commad buffers");
        }
    }

    void MageRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(mageDevice.device(), mageDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer MageRenderer::beginFrame() {
        assert(!isFrameStarted && "Can't call begin frame while already in progress");

        auto result = mageSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("falied to begin recording command buffers");
        }
        return commandBuffer;
    }

    void MageRenderer::endFrame() {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer");
        }

        auto result = mageSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mageWindow.wasWindowResized())
        {
            mageWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % MageSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void MageRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = mageSwapChain->getRenderPass();
        renderPassInfo.framebuffer = mageSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = mageSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValue{};
        clearValue[0].color = {0.2f, 0.2f, 0.2f, 1.0f};
        clearValue[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = clearValue.size();
        renderPassInfo.pClearValues = clearValue.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(mageSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(mageSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, mageSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void MageRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }

} // namespace mage
