#include "Types.hpp"
#include "Engine.hpp"

// Renderer
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "SimpleMaterial.hpp"
#include "Texture.hpp"

// Res
#include "MeshFactory.hpp"
#include "TextureLoader.hpp"

// STL
#include <iostream>


namespace Acroy
{
    Engine::Engine()
    {
        _window = std::make_unique<Window>(WindowProps {
            .width      = 1920,
            .height     = 1080,
            .fullscreen = true,
            .title      = "Acroy Engine"
        });

        _renderer = std::make_unique<Renderer>(RendererDesc {
            .window      = _window.get(),
            .enableVSync = true
        });

        _mesh = std::shared_ptr<Mesh>(
            MeshFactory::CreateQuad()
        );

        _mat = std::make_shared<SimpleMaterial>();

        _mat->SetTexture(
            std::shared_ptr<Texture>(TextureLoader::FromFile("/home/sam/Pictures/gruvbox/beach.jpg"))
        );

    }

    void Engine::Run()
    {
        while (!_window->ShouldClose())
        {
            _renderer->BeginFrame();
            _renderer->DrawMesh(_mesh.get(), _mat.get());
            _renderer->EndFrame();
            
            _window->Update();
        }
    }

    Engine::~Engine()
    {
        std::cout << "Shutting down engine..." << std::endl;
    }
}