#pragma once

#include <Core/Layer.hpp>
#include <Renderer/Camera.hpp>
#include <Core/Core.hpp>
#include <Scene/Entity.hpp>
#include "CameraController.hpp"

#include <string>
#include <vector>
#include <utility>
#include <optional>

class SandboxLayer : public Acroy::Layer
{
public:
    void OnAttach() override;
    void OnUpdate(Acroy::Timestep timestep) override;
    void OnEvent(Acroy::Event& event) override;
    void OnImGuiRender() override;

private:
    void DrawComponentsPanel();

    Acroy::Ref<Acroy::Scene> m_scene;
    std::vector<std::pair<std::string, Acroy::Entity>> m_entities;

    int m_selectedEntity = -1;
};