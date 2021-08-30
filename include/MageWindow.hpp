#pragma once

#include "slx.hpp"
#include "glx.hpp"
#include "NoCopy.hpp"

namespace mage
{
    class MageWindow : NoCopy
    {
    private:
        static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool frameBufferResized = false;

        std::string windowName;
        GLFWwindow *window;

    public:
        MageWindow(int w, int h, std::string name);
        ~MageWindow();

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return frameBufferResized; }
        void resetWindowResizedFlag() { frameBufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    };
} // namespace mage