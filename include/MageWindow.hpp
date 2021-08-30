#pragma once

#include "slx.hpp"
#include "glx.hpp"
#include "NoCopy.hpp"

namespace mage
{
    class MageWindow final : NoCopy
    {
    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;

    public:
        MageWindow(int w, int h, std::string name);
        ~MageWindow();

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    };
} // namespace mage