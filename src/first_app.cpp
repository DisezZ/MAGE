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
        MageCamera mageCamera{};

        while (!mageWindow.shouldClose())
        {
            glfwPollEvents();

            float aspect = mageRenderer.getAspectRatio();
            //mageCamera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            mageCamera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

            if (auto commandBuffer = mageRenderer.beginFrame())
            {
                mageRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, mageCamera);
                mageRenderer.endSwapChainRenderPass(commandBuffer);
                mageRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(mageDevice.device());
    }

    std::unique_ptr<MageModel> FirstApp::createCubeModel(MageDevice &device, glm::vec3 offset)
    {
        std::vector<MageModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto &v : vertices)
        {
            v.position += offset;
        }
        return std::make_unique<MageModel>(device, vertices);
    }

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<MageModel> mageModel = createCubeModel(mageDevice, {0.f, 0.f, 0.f});

        auto cube = MageGameObject::createGameObject();
        cube.model = mageModel;
        cube.transform.translation = {.0f, .0f, 2.5f};
        cube.transform.scale = {.5f, .5f, .5f};
        gameObjects.push_back(std::move(cube));
    }

} // namespace mage
