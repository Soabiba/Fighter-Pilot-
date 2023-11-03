#pragma once
#include <vector>
#include <iostream>
#include "raylib.h"
#include <string>

// Enumeration for game states
enum GameState
{
    MAIN_MENU,
    PLAY,
    HIGH_SCORE,
    GAME_OVER,
    COLLISION,
    WRITE_NAME,
    QUIT
};

// Enumeration for collision shapes
enum Shapes
{
    CIRCLE,
    SQUARE,
    LINE
};

class Level
{
public:

    // Screen Variables
    int screenWidth = 1250;
    int screenHeight = 600;

    // Major .CPP Files
    void Initialization();  // Initialize the game
    void Resources();      // Load game resources
    void Update();         // Update game logic
    void Draw();           // Render game elements
    void De_Initialization();  // Clean up and close the game

    // Level.CPP functions
    void Main_Menu();          // Handle main menu logic
    void UserMovementControl();   // Handle player movement and input
    void UpdateMainGame();         // Update the main game
    void Collision();  // Handle collision detection
    void CollisionUpdate();    // Update collision elements
    void DisplayHighScores();    // Display high scores
    void WritePlayerScore();         // Write the player's score
    void UpdatePlayerSprite();// Update the player's plane animation
    void GameOver();           // Handle game over screen
    void Quit();               // Quit the game

    // Draw.CPP functions
    void DrawMain_Menu();           // Render the main menu
    void RenderGameScreen();                // Render the game screen
    void DrawHighScore();           // Render the high score screen
    void DrawGameOver();            // Render the game over screen
    void DrawCollision();   // Render collision detection screen
    void DrawBackground();           // Render the background animation
    void DrawButtonsForCollision();  // Render collision shape buttons
    void DrawMovingShapes();         // Render moving collision shapes
    void DrawMidShape();             // Render the central collision shape
    void Drawplane();                // Render the player's plane

    // Initialization.CPP functions
    void PlayerInitialization();   // Initialize player variables
    void ButtonInitialization();   // Initialize button variables
    void ShapesInitialization();   // Initialize collision shapes

private:

    // Game Variables
    GameState gameState;
    Shapes shapes;

    bool gameOver;
    bool mainScreen;

    double Time;
    double passedTime;

    Vector2 mouseOffset;
    bool isDragging;

    // Buttons Variables & Functions
    struct Button
    {
        Rectangle rect;
        const char* text;
        bool isHighlighted;
    };

    Button StartButton, scoresButton, quitButton, backButton, retryButton, backToMain_MenuButton, collisionButton, circleButton, boxButton, lineButton;

    void DrawButton(Button button, bool isHighlighted);  // Render button

    // Background Animation Variables
    struct Background 
    {
        float scrollSpeed = 1.5f;
        float offsetX = 0.0f;
    };

    Background background;

    // Projectiles Functions & Variables
    struct Missiles 
    {
        Vector2 position;
        Vector2 velocity;
        Vector2 size = { 5, 2 };
        bool isActive;
    };

    std::vector<Missiles> missiles;

    float projectileSpeed;
    float shotTimer;
    float shotInterval;

    void ShootAutomatic();
    void ShootMissiles();
    void UpdateMissiles();
    void MissileEnemyCollisions();

    // Destructible enemies Variables & Functions
    struct Enemyplane 
    {
        Vector2 position = {};
        float speed = 0.0f;
        bool isDead = false;
        int width = 0;
        int height = 0;
    };

    std::vector<Enemyplane> enemyplanes;

    double lastEnemySpawnTime;
    double enemySpawnInterval = 5.0;

    void SpawnEnemyShip();

    // Non-destructible enemies Functions & Variables
    struct FasterEnemyPlanes 
    {
        Vector2 position = {};
        float speed = 0.0f;
        Color color = BLACK;
        float width = 0;
        float height = 0;
        bool isDead = false;
    };

    std::vector<FasterEnemyPlanes> fasterEnemyPlanes;

    void SpawnFasterEnemyPlanes();

    // Player Variables
    struct plane
    {
        Rectangle rec = { 0, 0, 50, 50 };
        int speed = 200;
        bool isDead;
        Vector2 position;
        Vector2 velocity;
    };

    plane plane, BlackHawk;

    int NUM_plane_FRAMES;
    int currentFrame;
    int frameWidth;
    Vector2 offsetplane;

    // Collision Functions & Variables
    struct Circle
    {
        int centerX;
        int centerY;
        float radius;
        bool appear;
        Color color;
    };

    Circle midCircle, movingCircle;

    struct Square
    {
        int posX;
        int posY;
        int width;
        int height;
        bool appear;
        Color color;
    };

    Square midSquare, movingSquare;

    struct Line
    {
        Vector2 startPos;
        Vector2 endPos;
        int halfsize;
        float thick;
        bool appear;
        Color color;
    };

    Line midLine, movingLine;

    bool CircleCloneCollision(const Circle& circle, const Square& box);
    bool CircleCollision(const Circle& midCircle, const Circle& movingCircle);
    bool CircleLineCollision(float centerX, float centerY, float circleRadius, float startPosX, float startPosY, float endPosX, float endPosY) const;
    bool CloneCollision(const Square& box1, const Square& box2);

    // Textures
    Texture2D backgroundImage; // Background Image
    Texture2D planeImage; // Plane Image
    Texture2D menuImage;
    Texture2D collisionBg;
    Texture2D highscoreImage;
    Texture2D enemyTexture;
    Texture2D fasterenemyTexture;
        
    // Sounds
    Sound hitSound;
    Sound gameOverSound;
    Sound hoveringSound;

    bool musicPlayed;

    // Highscore Functions & Variables
    struct HighScore
    {
        std::string name;
        int score = 0;
    };

    HighScore highscore;

    std::vector<HighScore> highScores;

#define MAX_INPUT_CHARS 3

    Rectangle textSquare = { screenWidth / 2.0f - 100, 180, 225, 50 };
    std::string name;

    int letterCount = 0;
    bool mouseOnText = false;
    int framesCounter = 0;
    bool nameConfirmed;

    void WriteHighScoreToFile();
    void Writename();
    void DrawWriteNameScore();
    void ReadHighScoresFromFile();
    bool CompareHighScores(const HighScore& a, const HighScore& b);
    bool IsHighScore(int currentScore);
};
