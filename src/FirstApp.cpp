#include "FirstApp.hpp"

namespace mage
{
    FirstApp::FirstApp()
    {
        loadModels();
        createPipelineLayout();
        createPipeline();
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
            {{0.0f, -0.5f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}}
        };

        mageModel = std::make_unique<MageModel>(mageDevice, vertices);
    }

    void FirstApp::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(mageDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }

    void FirstApp::createPipeline()
    {
        auto pipelineConfig = MagePipeline::defaultPipelineConfigInfo(mageSwapChain.width(), mageSwapChain.height());
        pipelineConfig.renderPass = mageSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        magePipeline = std::make_unique<MagePipeline>(
            mageDevice,
            "shaders/shaderSimple.vert.spv",
            "shaders/shaderSimple.frag.spv",
            pipelineConfig);
    }

    void FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(mageSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = mageDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(mageDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("falied to allocate commad buffers");
        }

        for (int i = 0; i < static_cast<int>(commandBuffers.size()); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("falied to begin recording command buffers");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = mageSwapChain.getRenderPass();
            renderPassInfo.framebuffer = mageSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = mageSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValue{};
            clearValue[0].color = {0.2f, 0.4f, 0.2f, 1.0f};
            clearValue[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = clearValue.size();
            renderPassInfo.pClearValues = clearValue.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            magePipeline->bind(commandBuffers[i]);
            mageModel->bind(commandBuffers[i]);
            mageModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to record command buffer");
            }
        }
    }

    void FirstApp::drawFrame()
    {
        uint32_t imageIndex;
        auto result = mageSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image");
        }

        result = mageSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image");
        }
    }

} // namespace mage
