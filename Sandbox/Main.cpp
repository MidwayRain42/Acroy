#include "Engine.hpp"
#include "Renderer.hpp"
#include "Events.hpp"
#include "Mesh.hpp"
#include "SimpleMaterial.hpp"
#include "MeshFactory.hpp"
#include "TextureLoader.hpp"
#include "Texture.hpp"
#include "PerspectiveCamera.hpp"
#include "Input.hpp"

#include <glm/glm.hpp>

class MyLayer : public Acroy::Layer {
private:
    Acroy::Mesh* m_mesh;
    Acroy::SimpleMaterial* m_material;
    Acroy::Texture* m_tex;
    Acroy::PerspectiveCamera m_cam;

    f32 m_rotation = 0.0f;
    Acroy::Transform m_meshTransform;

public:
    void OnAttach() override
    {
        m_mesh     = Acroy::MeshFactory::CreateCube();
        m_material = new Acroy::SimpleMaterial();

        m_tex = Acroy::TextureLoader::FromFile("/home/sam/Test/OpenGL-Sandbox/resources/textures/Planks/planks.png");

        m_material->SetTexture(m_tex);

        m_cam.SetPerspective(35.f, 16.f/9.f, .1f, 100.f);

        m_cam.GetTransform().Translate({0, 0, -3});
    }

    void OnEvent(Acroy::Event& event) override
    {
        switch (event.GetEventType())
        {
            default: break;
        }
    }

    void OnUpdate(f32 deltaTime) override
    {
        m_rotation += 1.0f * deltaTime;

        m_meshTransform.SetRotation(glm::vec3(m_rotation, m_rotation, 0));
    }

    void OnRender(Acroy::Renderer& renderer) override
    {
        renderer.SetCamera(m_cam);

        renderer.DrawMesh(m_mesh, m_material, m_meshTransform);
    }

    void OnDetach() override
    {
        delete m_mesh;
        delete m_material;
        delete m_tex;
    }
};

int main()
{
    Acroy::Engine engine;
    engine.PushLayer<MyLayer>();
    engine.Run();
    
    return EXIT_SUCCESS;
}
