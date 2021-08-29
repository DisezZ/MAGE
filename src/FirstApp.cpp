#include "FirstApp.hpp"

namespace mage
{
    FirstApp::FirstApp()
    {
    }

    FirstApp::~FirstApp()
    {
    }

    void FirstApp::run()
    {
        while(!mageWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }
} // namespace mage
