#include "raylib.h"
#include "Level.h"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    SetTargetFPS(60);

    Level level;

    level.Initialization();

    InitWindow(level.screenWidth, level.screenHeight, "Fighter Pilot");

    InitAudioDevice();

    level.Resources();
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        level.Update();
        level.Draw();

    }

    level.De_Initialization();


    return 0;
}