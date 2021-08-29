#include "MageWindow.hpp"

namespace mage
{
    MageWindow::MageWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    MageWindow::~MageWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void MageWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

    void MageWindow::createWindowSurface(VkInstance instace, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instace, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface");
        }
    }
}