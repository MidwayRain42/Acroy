#include <Engine.hpp>
#include "Game.hpp"

int main()
{
    Acroy::Engine engine;
    engine.PushLayer<Game>();
    engine.Run();

    return 0;
}