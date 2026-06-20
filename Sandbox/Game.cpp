#include "Game.hpp"
#include <Renderer.hpp>
#include <iostream>
#include <ResourceManager.hpp>

void Game::OnAttach()
{
    LoadResources();

    m_camera.SetPerspective(90.0f, 16.0f/9.0f, 0.01f, 100.0f);

    m_controller.SetCamera(m_camera);
    m_controller.SetMoveSpeed(3.0f);
    m_controller.SetSensitivity(0.0015f);
}

void Game::OnEvent(Acroy::Event& e)
{
    m_controller.OnEvent(e);
}

void Game::OnUpdate(f32 deltaTime)
{
    m_controller.OnUpdate(deltaTime);
}

void Game::OnRender(Acroy::Renderer& renderer)
{
    renderer.SetCamera(m_camera);

    renderer.DrawMesh(*m_chair, *m_chairMat, Acroy::Transform{});
    
    Acroy::Transform t;
    t.SetScale({15.0f, 1.0f, 15.0f});

    renderer.DrawMesh(*m_ground, *m_groundMat, t);


    renderer.DrawSkybox(*m_sky);
}

void Game::OnDetach()
{
}

void Game::LoadResources()
{
    m_chair  = Acroy::ResourceManager::LoadOBJ("Sandbox/Resources/Models/Chair.obj");
    m_ground = Acroy::ResourceManager::LoadOBJ("Sandbox/Resources/Models/Plane.obj");

    m_chairMat  = std::make_shared<Acroy::SimpleMaterial>();
    m_groundMat = std::make_shared<Acroy::SimpleMaterial>();

    m_chairTex  = Acroy::ResourceManager::LoadTexture("Sandbox/Resources/Models/Chair.jpg");
    m_groundTex = Acroy::ResourceManager::LoadTexture("Sandbox/Resources/Textures/Marble.jpg");

    m_chairMat->SetTexture(m_chairTex.get());
    m_groundMat->SetTexture(m_groundTex.get());

    m_groundMat->SetUVScale({22.5f, 22.5f});

    std::array<std::string_view, 6> cubemap =
    {
        "Sandbox/Resources/Textures/skybox/px.png",
        "Sandbox/Resources/Textures/skybox/nx.png",
        "Sandbox/Resources/Textures/skybox/py.png",
        "Sandbox/Resources/Textures/skybox/ny.png",
        "Sandbox/Resources/Textures/skybox/pz.png",
        "Sandbox/Resources/Textures/skybox/nz.png",
    };

    m_sky = std::make_unique<Acroy::Skybox>(cubemap);
}
