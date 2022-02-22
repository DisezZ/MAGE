#pragma once

#include "slx.hpp"
#include "glx.hpp"

#include "no_copy.hpp"
#include "mage_window.hpp"
#include "mage_device.hpp"
#include "mage_game_object.hpp"
#include "mage_renderer.hpp"

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
        void loadGameObjects();

        MageWindow mageWindow{WIDTH, HEIGHT, "Vulkan::Mage Engine"};
        MageDevice mageDevice{mageWindow};
        MageRenderer mageRenderer{mageWindow, mageDevice};
        
        std::vector<MageGameObject> gameObjects;
    };
} // namespace mage
