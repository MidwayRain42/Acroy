#include "Types.hpp"
#include "Engine.hpp"

// Renderer
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "SimpleMaterial.hpp"

// Res
#include "MeshFactory.hpp"
#include "TextureLoader.hpp"

// STL
#include <iostream>


namespace Acroy
{
    Engine::Engine()
    {
        _window = new Window({
            .width = 1600,
            .height = 900,
            .fullscreen = false,
            .title = "Acroy Engine"
        });

        _renderer = new Renderer({
            .window = _window,
            .enableVSync = true
        });

        _mesh = MeshFactory::CreateQuad();

        _mat = new SimpleMaterial();

        _mat->SetTexture(
            TextureLoader::FromFile("/home/sam/Pictures/Wallpapers/ayam7gtjz0r91.jpg")
        );
    }

    void Engine::Run()
    {
        while (!_window->ShouldClose())
        {
            _renderer->BeginFrame();
            _renderer->DrawMesh(_mesh, _mat);
            _renderer->EndFrame();
            
            _window->Update();
        }
    }

    Engine::~Engine()
    {
        std::cout << "Shutting down engine..." << std::endl;

        /*

        This is temporary, in the future there will
        be a reference counting system that will automatically
        delete unused resources.

        */

        // delete _mesh;
        // delete _mat;
        // delete _renderer;
        // delete _window;
    }
}