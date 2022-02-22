#include <slx.hpp>

#include "first_app.hpp"
#include "simple_render_system.hpp"

namespace mage
{

    FirstApp::FirstApp()
    {
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run()
    {
        SimpleRenderSystem simpleRenderSystem{mageDevice, mageRenderer.getSwapChainRenderPass()};

        while (!mageWindow.shouldClose())
        {
            glfwPollEvents();
            
            if (auto commandBuffer = mageRenderer.beginFrame())
            {
                mageRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                mageRenderer.endSwapChainRenderPass(commandBuffer);
                mageRenderer.endFrame();
            }
            
        }

        vkDeviceWaitIdle(mageDevice.device());
    }

    void FirstApp::loadGameObjects()
    {
        std::vector<MageModel::Vertex> vertices{
            {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

        auto mageModel = std::make_shared<MageModel>(mageDevice, vertices);

        auto triangle = MageGameObject::createGameObject();
        triangle.model = mageModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform.translation.x = .2f;
        triangle.transform.scale = {2.f, 0.5f, 1.f};
        triangle.transform.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

} // namespace mage
