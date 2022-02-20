#include "mage_game_object.hpp"

namespace mage
{
    glm::mat4 TransformComponent::mat4() {
        scale = glm::vec3{2.f, 1.f, 1.f};

        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat4 rotMat{{c, s, 0.f, 0.f}, {-s, c, 0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 1.f}};
        //glm::mat4 rotMat{1.f};
        glm::mat4 scaleMat{{scale.x, 0.f, 0.f, 0.f}, {0.f, scale.y, 0.f, 0.f}, {0.f, 0.f, scale.z, 0.f}, {0.f, 0.f, 0.f, 1.f}};
        return rotMat * scaleMat; 
    }
} // namespace mage
