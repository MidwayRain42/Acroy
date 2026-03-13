#pragma once

#include "Renderer/Mesh.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Camera.hpp"
#include "ScriptableEntity.hpp"
#include <glm/glm.hpp>

namespace Acroy
{
    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;
        TagComponent(const std::string& tag) : tag(tag) {}
    };

    struct TransformComponent
    {
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f};
        glm::vec3 scale {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;

        TransformComponent(const glm::vec3& pos)
            : position(pos) {}

        glm::mat4 GetTransform() const
        {
            glm::mat4 rot =
            glm::rotate(glm::mat4(1.0f), rotation.z, {0,0,1}) *
            glm::rotate(glm::mat4(1.0f), rotation.y, {0,1,0}) *
            glm::rotate(glm::mat4(1.0f), rotation.x, {1,0,0});

            return glm::translate(glm::mat4(1.0f), position)
                * rot
                * glm::scale(glm::mat4(1.0f), scale);
        }

        operator glm::mat4() const { return GetTransform(); }
    };

    struct MeshComponent
    {
        Ref<Mesh> mesh;

        MeshComponent() = default;
        MeshComponent(const Ref<Mesh> mesh) : mesh(mesh) {}

        operator const Ref<Mesh>&() const { return mesh; }
    };

    struct MaterialComponent
    {
        Ref<Shader> shader;
        
        glm::vec4 albedo { 1.0f };
        Ref<Texture> albedoTex;

        float scale = 1.0f;

        MaterialComponent(const Ref<Shader>& shader) : shader(shader) {}
        MaterialComponent() = default;
    };

    struct CameraComponent
    {
        Ref<Camera> camera;
        bool primary;

        CameraComponent() = default;
        CameraComponent(const Ref<Camera> cam, bool primary = true)
        : camera(cam), primary(primary) {}

    };

    struct NativeScriptComponent
    {
        ScriptableEntity* instance = nullptr;
        ScriptableEntity* (*InstaniateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstaniateScript = []() {
                return static_cast<ScriptableEntity*>(new T());
            };

            DestroyScript = [](NativeScriptComponent* nsc) {
                delete nsc->instance;
                nsc->instance = nullptr;
            };
        }

    };
}
