#pragma once

#include "Entity.hpp"
#include <optional>

namespace Acroy
{
    class ScriptableEntity
    {
    public:
        ScriptableEntity() = default;
        virtual ~ScriptableEntity() = default;

        template<typename T>
        T& GetComponent()
        {
            return m_entity.value().GetComponent<T>();
        }

    protected:
        virtual void OnStart() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnDestroy() {}

    private:
        std::optional<Entity> m_entity;
        friend class Scene;
    };
}
