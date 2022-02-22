#include "mage_game_object.hpp"

namespace mage
{
    glm::mat4 TransformComponent::mat4() {
        glm::mat4 translateMat = glm::translate(glm::mat4{1.f}, translation);
        glm::mat4 rotateMat = glm::toMat4(glm::quat{rotation});
        glm::mat4 scaleMat = glm::scale(glm::mat4{1.f}, scale);

        glm::mat4 transform = translateMat * rotateMat * scaleMat;
        
        return transform;
    }
    
} // namespace mage
