#include "slx.hpp"
#include "glx.hpp"
#include "FirstApp.hpp"

int main()
{
    mage::FirstApp app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}