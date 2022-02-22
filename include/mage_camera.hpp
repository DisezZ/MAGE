#pragma once

#include "slx.hpp"
#include "glx.hpp"

#include "no_copy.hpp"

namespace mage
{
    class MageCamera
    {
    public:
        MageCamera();
        ~MageCamera();

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovY, float aspect, float near, float far);

        const glm::mat4 &getProjection() const { return projectionMatrix; }

    private:
        glm::mat4 projectionMatrix{1.f};
    };

} // namespace mage
