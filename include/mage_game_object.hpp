#pragma once

#include "slx.hpp"
#include "glx.hpp"
#include "no_copy.hpp"

#include "mage_model.hpp"

// std
#include <memory>

namespace mage
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        glm::mat4 mat4();
    };

    class MageGameObject
    {
    public:
        using id_t = unsigned int;
        
        static MageGameObject createGameObject() {
            static id_t currentId = 0;
            return MageGameObject{currentId++};
        }
        
        MageGameObject(const MageGameObject &&) = delete;
        MageGameObject &operator=(const MageGameObject &&) = delete;
        MageGameObject(MageGameObject &&) = default;
        MageGameObject &operator=(MageGameObject &&) = default;

        id_t getId() { return id; }

        std::shared_ptr<MageModel> model{};
        glm::vec3 color{};
        TransformComponent transform;

    private:
        MageGameObject(id_t objId) : id{objId} {};

        id_t id;
    };
} // namespace mage
