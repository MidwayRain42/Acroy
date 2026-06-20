#pragma once

#include "CameraController.hpp"

#include <Layer.hpp>
#include <PerspectiveCamera.hpp>
#include <Mesh.hpp>
#include <SimpleMaterial.hpp>
#include <Texture.hpp>
#include <memory>
#include <Skybox.hpp>

class Game : public Acroy::Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Acroy::Event& event) override;
    void OnUpdate(f32 deltaTime) override;
    void OnRender(Acroy::Renderer& renderer) override;

private:
    CameraController m_controller;

    Acroy::PerspectiveCamera m_camera;

    std::unique_ptr<Acroy::Skybox>         m_sky;
    
    std::shared_ptr<Acroy::Mesh>           m_chair;
    std::shared_ptr<Acroy::Mesh>           m_ground;
    std::shared_ptr<Acroy::SimpleMaterial> m_chairMat;
    std::shared_ptr<Acroy::SimpleMaterial> m_groundMat;
    std::shared_ptr<Acroy::Texture>        m_chairTex;
    std::shared_ptr<Acroy::Texture>        m_groundTex;

private:
    void LoadResources();
};
