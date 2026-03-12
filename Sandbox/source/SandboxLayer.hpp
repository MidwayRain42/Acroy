#pragma once

#include <Core/Layer.hpp>
#include <Renderer/Camera.hpp>
#include <Core/Core.hpp>
#include <Scene/Entity.hpp>
#include "CameraController.hpp"

#include <string>
#include <vector>
#include <utility>

class SandboxLayer : public Acroy::Layer
{
public:
    void OnAttach() override;
    void OnUpdate(Acroy::Timestep timestep) override;
    void OnEvent(Acroy::Event& event) override;
    void OnImGuiRender() override;

private:
    Acroy::Ref<Acroy::Scene> m_scene;

    // Named entity handles used by the Scene Inspector panel.
    // vector<pair> avoids the default-constructor requirement that
    // unordered_map::operator[] imposes on its value type.
    std::vector<std::pair<std::string, Acroy::Entity>> m_entities;
};