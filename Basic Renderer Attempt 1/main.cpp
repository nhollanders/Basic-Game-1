// Project Specific Includes (custom)
#include "Game.h"

int main()
{
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