#include "raylib.h"
#include "Level.h"

void Level::De_Initialization()
{
    // Unload all the Images 

    UnloadTexture(backgroundImage);
    UnloadTexture(planeImage);
    UnloadTexture(menuImage);
    UnloadTexture(collisionBg);
    UnloadTexture(highscoreImage);

    // Unload all the Sounds
    
    UnloadSound(hitSound);
    UnloadSound(gameOverSound);
    UnloadSound(hoveringSound);

    CloseAudioDevice();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

}