#pragma once

#include "MageWindow.hpp"
#include "MagePipeline.hpp"
#include "MageDevice.hpp"
#include "NoCopy.hpp"

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
        MageWindow mageWindow{WIDTH, HEIGHT, "Vulkan::Mage Engine"};
        MageDevice mageDevice{mageWindow};
        MagePipeline magePipeline{mageDevice, "shaders/shaderSimple.vert.spv", "shaders/shaderSimple.frag.spv", MagePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
} // namespace mage
