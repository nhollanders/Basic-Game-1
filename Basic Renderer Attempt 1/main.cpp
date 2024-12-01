// Project Specific Includes (custom)
#include "Game.h"

int main()
{
    // SRand seed initialization
    std::srand(static_cast<unsigned>(time(NULL)));

    // Init game engine
    Game game;

    game.setDisplayTitleFps(true); 

    while (game.getIsRunning())
    {
        // Update
        game.update();
        
        // Render
        game.render();
    }

    return 0;
}