#include "Engine.hpp"
#include "Renderer.hpp"
#include "Events.hpp"
#include "Mesh.hpp"
#include "SimpleMaterial.hpp"
#include "MeshFactory.hpp"

class MyLayer : public Acroy::Layer {
private:
    Acroy::Mesh* _mesh;
    Acroy::SimpleMaterial* _material;
    bool _draw = false;

public:
    void OnAttach() override
    {
        _mesh     = Acroy::MeshFactory::CreateQuad();
        _material = new Acroy::SimpleMaterial();

        f32 color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        _material->SetColor(color);
    }

    void OnEvent(Acroy::Event& event) override
    {
        switch (event.GetEventType())
        {
            case Acroy::EventType::MouseButtonPressed:  _draw = true;  break;
            case Acroy::EventType::MouseButtonReleased: _draw = false; break;

            default: break;
        }
    }

    void OnRender(Acroy::Renderer& renderer) override
    {
        if (_draw)
            renderer.DrawMesh(_mesh, _material);
    }
};

int main()
{
    Acroy::Engine engine;
    engine.PushLayer<MyLayer>();
    engine.Run();
    
    return EXIT_SUCCESS;
}
