#include "AcroyPCH.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Core/Log.hpp"
#include "Renderer/Renderer.hpp"
#include "Components.hpp"

namespace Acroy
{
    Scene::Scene(const Ref<Camera>& cam) : m_camera(cam) {}

    Entity Scene::Create(const std::string& name)
    {
        Entity entity = { m_registry.create(), this };

        TagComponent& tag = entity.AddComponent<TagComponent>();
        tag = name.empty() ? "Object" : name;

        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        // Get Primary Camera
        Ref<Camera> cam;
        TransformComponent* camTransform = nullptr;

        auto camView = m_registry.view<TransformComponent, CameraComponent>();

        for (auto entity : camView)
        {
            auto& transform = camView.get<TransformComponent>(entity);
            auto& camera = camView.get<CameraComponent>(entity);

            if (camera.primary)
            {
                cam = camera.camera;
                camTransform = &transform;
                break;
            }
        }

        if (!cam)
        {
            ACROY_CORE_INFO("No Primary Camera in Scene");
            return;
        }
        
        // Rendering
        Renderer::BeginScene({ cam->GetProjection(), glm::inverse(camTransform->GetTransform()) });
        
        auto renderView = m_registry.group<TransformComponent, MeshComponent, MaterialComponent>();
        for (auto entity : renderView)
        {
            auto [transform, mesh, material] = renderView.get<TransformComponent, MeshComponent, MaterialComponent>(entity);

            material.shader->Bind();
            
            if (material.albedoTex)
            {
                material.albedoTex->Bind(0);
                material.shader->SetUniformInt("u_useTexture", 1);
                material.shader->SetUniformInt("u_texture", 0);
                material.shader->SetUniformFloat("u_textureScale", material.scale);
            }
            else
            {
                material.shader->SetUniformInt("u_useTexture", 0);
                material.shader->SetUniformFloat4("u_color", material.albedo);
            }
            
            Renderer::Submit(mesh, material.shader, transform);
        }

        Renderer::EndScene();

        // Update Scripts
        auto scriptView = m_registry.view<NativeScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& ncs = scriptView.get<NativeScriptComponent>(entity);

            if (!ncs.instance)
            {
                ncs.instance = ncs.InstaniateScript();
                ncs.instance->m_entity = Entity(entity, this);
                ncs.instance->OnStart();
            }

            ncs.instance->OnUpdate(ts);
        }

    }

    void Scene::OnWindowResize(float width, float height)
    {
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
            cameraComponent.camera->Resize(width, height);
		}
    }
}
