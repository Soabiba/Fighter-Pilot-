#include "Level.h"
#include "raylib.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>




// Main Upate function

void Level::Initialization()
{

    // Initialize game state and variables

    gameState = MAIN_MENU;
    highscore.score = 0;
    Time = GetTime();
    gameOver = false;
    nameConfirmed = false;
    currentFrame = 0;
    NUM_plane_FRAMES = 5;
    frameWidth = planeImage.width / NUM_plane_FRAMES;

    //  Initialize mouse dragging variables (for collision detaction)

    mouseOffset = { 0, 0 };
    isDragging = false;

    // Initialize Enemy Spanw variables

    lastEnemySpawnTime = GetTime();
    enemySpawnInterval = 1.0;
    enemyplanes.clear();

    musicPlayed = false;

    // Initialize buttons and shapes

    PlayerInitialization();

    // Initialize buttons and shapes

    ButtonInitialization();
    ShapesInitialization();


    // Read high scores from a file

    ReadHighScoresFromFile();
}


// initialiazation of plane structure

void Level::PlayerInitialization()
{

    plane.isDead = false;
    plane.rec.x = 100;
    plane.rec.y = (screenHeight - plane.rec.height) / 2;
    plane.speed = 5;
    offsetplane = { 70,70 };


    //shooting
    shotTimer = 0.0f;
    shotInterval = 0.5f;
    projectileSpeed = 4.0f;
}

// Initialize buttons for the game menu

void Level::ButtonInitialization()
{

    StartButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 - 25, 200, 60 }, "Start Game", false };
    scoresButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 50, 200, 60 }, "High Score", false };
    collisionButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 - 100, 200, 60 }, "Test Collisions", false };
    quitButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 125, 200, 60 }, "Quit", false };
    backButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 50, 200, 60 }, "Back", false };
    retryButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 125, 200, 60 }, "Back to Game", false };
    backToMain_MenuButton = { { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 200, 200, 60 }, "Main Menu", false };

    circleButton = { { 0, (float)screenHeight / 2 - 100, 200, 60 }, "Circle", false };
    boxButton = { { 0 , (float)screenHeight / 2 - 200, 200, 60 }, "Square", false };
    lineButton = { { 0 , (float)screenHeight / 2 - 300, 200, 60 }, "Line", false };

}

// Initialize of shapes used in collision detection

void Level::ShapesInitialization()
{
    midCircle.centerX = screenWidth / 2;
    midCircle.centerY = screenHeight / 2;
    midCircle.radius = 50;
    midCircle.color = DARKGREEN;
    midCircle.appear = false;


    midSquare.width = 100;
    midSquare.height = 100;
    midSquare.posX = screenWidth / 2 - midSquare.width / 2;
    midSquare.posY = screenHeight / 2 - midSquare.height / 2;
    midSquare.color = DARKGREEN;
    midSquare.appear = false;


    midLine.halfsize = 50;
    midLine.startPos = { (float)screenWidth / 2, (float)screenHeight / 2 + midLine.halfsize };
    midLine.endPos = { (float)screenWidth / 2, (float)screenHeight / 2 - midLine.halfsize };
    midLine.thick = 1;
    midLine.color = DARKGREEN;
    midLine.appear = false;

    movingCircle.centerX = screenWidth - 150;
    movingCircle.centerY = screenHeight / 2 - 200;
    movingCircle.radius = 25;
    movingCircle.color = SKYBLUE;
    movingCircle.appear = false;

    movingSquare.width = 50;
    movingSquare.height = 50;
    movingSquare.posX = screenWidth - 150;
    movingSquare.posY = screenHeight / 2 - movingSquare.height / 2;
    movingSquare.color = SKYBLUE;
    movingSquare.appear = false;

}


// Read high scores from a file and store them in the highScores vector

void Level::ReadHighScoresFromFile() 
{


    std::ifstream highScoreFile("./Source/Assets/HighScore.txt");
    if (highScoreFile.is_open()) 
    {
        highScores.clear();
        for (int i = 0; i < 5; i++) 
        {
            HighScore score;
            if (highScoreFile >> score.score >> score.name) 
            {
                highScores.push_back(score);
            }
        }
        highScoreFile.close();
    }
}