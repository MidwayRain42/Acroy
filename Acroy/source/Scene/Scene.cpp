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
        
        Renderer::BeginScene({ cam->GetProjection(), glm::inverse(camTransform->GetTransform()) });
        
        auto view = m_registry.group<TransformComponent, MeshComponent, MaterialComponent>();
        for (auto entity : view)
        {
            auto [transform, mesh, material] = view.get<TransformComponent, MeshComponent, MaterialComponent>(entity);

            material.shader->Bind();

            if (material.albedoTex)
            {
                material.albedoTex->Bind(0);
                material.shader->SetUniformInt("u_texture", 0);
                material.shader->SetUniformFloat("u_textureScale", material.scale);
            }
            
            Renderer::Submit(mesh, material.shader, transform);
        }

        Renderer::EndScene();
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
