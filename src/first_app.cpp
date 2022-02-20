#include "first_app.hpp"

namespace mage
{
    struct PushConstantData {
        alignas(16) glm::vec3 offset;
        alignas(16) glm::vec3 color;
    };

    FirstApp::FirstApp()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(mageDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run()
    {
        while (!mageWindow.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(mageDevice.device());
    }

    void FirstApp::loadModels()
    {
        std::vector<MageModel::Vertex> vertices{
            {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

        mageModel = std::make_unique<MageModel>(mageDevice, vertices);
    }

    void FirstApp::createPipelineLayout()
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

    void FirstApp::createPipeline()
    {
        assert(mageSwapChain != nullptr && "can't create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "can't create pipeline before pipeline layout");
        PipelineConfigInfo pipelineConfig{};
        MagePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = mageSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        magePipeline = std::make_unique<MagePipeline>(
            mageDevice,
            "shaders/shaderSimple.vert.spv",
            "shaders/shaderSimple.frag.spv",
            pipelineConfig);
    }

    void FirstApp::recreateSwapChain()
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
            mageSwapChain = std::make_unique<MageSwapChain>(mageDevice, extent, std::move(mageSwapChain));
            if (mageSwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        // if render pass compatible do nothing else recreate pipeline
        createPipeline();
    }

    void FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(mageSwapChain->imageCount());

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

    void FirstApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(mageDevice.device(), mageDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    void FirstApp::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("falied to begin recording command buffers");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = mageSwapChain->getRenderPass();
        renderPassInfo.framebuffer = mageSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = mageSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValue{};
        clearValue[0].color = {0.2f, 0.2f, 0.2f, 1.0f};
        clearValue[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = clearValue.size();
        renderPassInfo.pClearValues = clearValue.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(mageSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(mageSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, mageSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        magePipeline->bind(commandBuffers[imageIndex]);
        mageModel->bind(commandBuffers[imageIndex]);
        for (int j = 0; j < 4; j++)
        {
            PushConstantData push{};
            push.offset = glm::vec3(0.0f, -0.4 + j * 0.25, 0.0f);
            push.color = glm::vec3(0.0f, 0.0f, 0.2f + 0.2f * j);

            vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &push);
            mageModel->draw(commandBuffers[imageIndex]);
        }

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer");
        }
    }

    void FirstApp::drawFrame()
    {
        uint32_t imageIndex;
        auto result = mageSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image");
        }

        recordCommandBuffer(imageIndex);
        result = mageSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mageWindow.wasWindowResized())
        {
            mageWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image");
        }
    }

} // namespace mage
