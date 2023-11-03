#include "raylib.h"
#include "Level.h"

#include <iostream>
#include <fstream>
#include <cmath>


void Level::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw based on the current game state
    if (gameState == MAIN_MENU)
    {
        DrawMain_Menu();
    }
    else if (gameState == PLAY)
    {
        RenderGameScreen();
    }
    else if (gameState == HIGH_SCORE)
    {
        DrawHighScore();
    }
    else if (gameState == GAME_OVER)
    {
        DrawGameOver();
    }
    else if (gameState == WRITE_NAME)
    {
        DrawWriteNameScore();
    }
    else if (gameState == COLLISION)
    {
        DrawCollision();
    }

    EndDrawing();
}

// Draw main menu screen with buttons

void Level::DrawMain_Menu()
{
    DrawTexture(menuImage, -22, -100, WHITE);

    DrawButton(StartButton, StartButton.isHighlighted);
    DrawButton(scoresButton, scoresButton.isHighlighted);
    DrawButton(collisionButton, collisionButton.isHighlighted);
    DrawButton(quitButton, quitButton.isHighlighted);
}

// Draw the main game screen

void Level::RenderGameScreen()
{
    // Draw the background
    DrawBackground();

    // Draw the player character
    Drawplane();

    //Draw Projectiles
    for (const auto& proj : missiles) {
        if (proj.isActive) {
            DrawRectangle(static_cast<int>(proj.position.x), static_cast<int>(proj.position.y), static_cast<int>(proj.size.x), static_cast<int>(proj.size.y), YELLOW);
        }
    }


    // Draw Enemies
    for (const auto& enemy : enemyplanes) {
        if (!enemy.isDead) {
            DrawTexture(enemyTexture, static_cast<int>(enemy.position.x), static_cast<int>(enemy.position.y), WHITE);
        }
    }

    // Draw enemies that cannot be destoyed
    for (const auto& InEnemy : fasterEnemyPlanes) {
        if (!InEnemy.isDead) {
            DrawTexture(fasterenemyTexture, static_cast<int>(InEnemy.position.x), static_cast<int>(InEnemy.position.y), WHITE);
        }
    }

    // Draw score top right corner

    DrawText(TextFormat("Score: %d", highscore.score), screenWidth/2 -50, 550, 30, DARKPURPLE);


}


// Read highscores from HighScore file and display them

void Level::DrawHighScore()
{
    DrawTexture(highscoreImage, -(int)background.offsetX, 0, WHITE);

    std::ifstream highScoreFile("./Source/Assets/HighScore.txt");
    std::string highScoreText;
    std::string scoreEntry;

    if (highScoreFile.is_open())
    {
        int lineNumber = 1;
        while (std::getline(highScoreFile, scoreEntry))
        {
            highScoreText += std::to_string(lineNumber) + ": " + scoreEntry + "\n";
            lineNumber++;
        }
        highScoreFile.close();
    }

    DrawText(highScoreText.c_str(), screenWidth/2-80, screenHeight/2-150, 30, ORANGE);


    DrawButton(retryButton, retryButton.isHighlighted);
    DrawButton(backToMain_MenuButton, backButton.isHighlighted);
}

// Draw Gameover screen

void Level::DrawGameOver()
{
    DrawTexture(collisionBg, -22, -100, WHITE);
    if (!musicPlayed)
    {
        PlaySound(gameOverSound);
        musicPlayed = true; // Set the flag to true to indicate that the music has been played
    }
    

    DrawButton(retryButton, retryButton.isHighlighted);
    DrawButton(backToMain_MenuButton, backButton.isHighlighted);
}

// Draw Collision Detection screen

void Level::DrawCollision()
{
    DrawTexture(collisionBg, -22, -100, WHITE);
    DrawButtonsForCollision();
    DrawMovingShapes();
    DrawMidShape();
    DrawButton(backToMain_MenuButton, backButton.isHighlighted);
}

//Draw the animation of the Game Background

void Level::DrawBackground()
{
    background.offsetX += background.scrollSpeed;

    if (background.offsetX >= backgroundImage.width) background.offsetX = 0;

    DrawTexture(backgroundImage, -(int)background.offsetX, 0, WHITE);
    DrawTexture(backgroundImage, -(int)background.offsetX + backgroundImage.width, 0, WHITE);
}

// Draw the buttons for choosing the collision shapes

void Level::DrawButtonsForCollision()
{
    DrawButton(circleButton, circleButton.isHighlighted);
    DrawButton(boxButton, boxButton.isHighlighted);
    DrawButton(lineButton, lineButton.isHighlighted);
}

// Draw the shapes for collisin that can move

void Level::DrawMovingShapes()
{
    if (movingCircle.appear) DrawCircle(movingCircle.centerX, movingCircle.centerY, movingCircle.radius, movingCircle.color);
    if (movingSquare.appear) DrawRectangle(movingSquare.posX, movingSquare.posY, movingSquare.width, movingSquare.height, movingSquare.color);
}

// Draw the shapes that are in the middle of the screen for collision detection

void Level::DrawMidShape()
{
    switch (shapes)
    {
    case CIRCLE:

        if (midCircle.appear) DrawCircle(midCircle.centerX, midCircle.centerY, midCircle.radius, midCircle.color);
        break;

    case SQUARE:
        if (midSquare.appear) DrawRectangle(midSquare.posX, midSquare.posY, midSquare.width, midSquare.height, midSquare.color);
        break;
    case LINE:
        if (midLine.appear) DrawLineEx(midLine.startPos, midLine.endPos, midLine.thick, midLine.color);
        break;
    }
}

// General function for Drawing buttons

void Level::DrawButton(Button button, bool isHighlighted)
{
    if (isHighlighted)
    {
        PlaySound(hoveringSound);
    }
    // Draw the button
    Color gold = { 255,215,0, 100 };
    DrawRectangleRec(button.rect, isHighlighted ? LIGHTGRAY : gold);

    // Calculate the text position to center it in the button
    int textWidth = MeasureText(button.text, 23);
    int textX = static_cast<int>(button.rect.x + (button.rect.width - textWidth) / 2);
    int textY = static_cast<int>(button.rect.y + (button.rect.height - 23) / 2);

    // Draw the text inside the button
    Color blue = { 25, 25, 112, 200 };
    DrawText(button.text, textX, textY, 23, blue);
}

// Draw The animation for the player

void Level::Drawplane()
{
    Rectangle sourceRect = { (float)frameWidth * currentFrame, 0, (float)frameWidth, (float)planeImage.height };
    DrawTextureRec(planeImage, sourceRect, { plane.rec.x - offsetplane.x, plane.rec.y - offsetplane.y }, WHITE);
}

// Draw the name writing window 

void Level::DrawWriteNameScore()
{
    if (nameConfirmed)
    {
        // Display the player name and score

        std::string playerNameAndScore = "Name: " + name + " Score: " + std::to_string(highscore.score);
        DrawText(playerNameAndScore.c_str(), 240, 140, 30, GRAY);
    }
    else
    {
        DrawText("Enter 3 initials and press Enter to confirm.", 240, 140, 30, GRAY);
        DrawRectangleRec(textSquare, LIGHTGRAY);

        if (mouseOnText)
        {
            DrawRectangleLines(static_cast<int>(textSquare.x), static_cast<int>(textSquare.y), static_cast<int>(textSquare.width), static_cast<int>(textSquare.height), DARKGREEN);
            DrawText(name.c_str(), static_cast<int>(textSquare.x + 5), static_cast<int>(textSquare.y + 8), 40, MAROON);

            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore character

                if (((framesCounter / 20) % 2) == 0)
                {
                    DrawText("_", static_cast<int>(textSquare.x + 8 + MeasureText(name.c_str(), 40)), static_cast<int>(textSquare.y + 12), 40, SKYBLUE);
                }
            }
        }
        else
        {
            DrawRectangleLines(static_cast<int>(textSquare.x), static_cast<int>(textSquare.y), static_cast<int>(textSquare.width), static_cast<int>(textSquare.height), DARKGRAY);
            DrawText(name.c_str(), static_cast<int>(textSquare.x + 5), static_cast<int>(textSquare.y + 8), 40, MAROON);
        }
    }
}