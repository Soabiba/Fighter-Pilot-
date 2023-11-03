#include "Level.h"
#include "raylib.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>


// Main Udate function

void Level::Update() 
{
    if (gameState == MAIN_MENU) 
    {
        Main_Menu(); // Menu screen
    }
    else if (gameState == PLAY) 
    {
        // Functions for Game screen

        UserMovementControl();
        UpdateMainGame();
        UpdatePlayerSprite();
        ShootAutomatic();
        UpdateMissiles();

    }
    else if (gameState == HIGH_SCORE) {
        DisplayHighScores(); // Highscore screen
    }
    else if (gameState == COLLISION) {
        Collision(); // Collision Detection screen
    }
    else if (gameState == GAME_OVER) {
        // Play the game over sound when entering the game over state
        if (!musicPlayed) {
            PlaySound(gameOverSound);
            musicPlayed = true;
        }
        GameOver(); // Game over screen
        
    }
    else if (gameState == WRITE_NAME) {
        WritePlayerScore(); // Screen for writting name after highscore is achieved
    }
    else if (gameState == QUIT) {
        Quit();
    }
}



// Menu screen 

void Level::Main_Menu() {

    // Check if mouse clicks on the buttons and then moves to the selected screen

    if (CheckCollisionPointRec(GetMousePosition(), StartButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Initialization();
        gameState = PLAY;
    }

    else if (CheckCollisionPointRec(GetMousePosition(), scoresButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = HIGH_SCORE;
    }

    else if (CheckCollisionPointRec(GetMousePosition(), collisionButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = COLLISION;
    }


    else if (CheckCollisionPointRec(GetMousePosition(), quitButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = QUIT;
    }

    // Check if the buttons are Highlighted when the mouse is om top of them

    StartButton.isHighlighted = CheckCollisionPointRec(GetMousePosition(), StartButton.rect);
    scoresButton.isHighlighted = CheckCollisionPointRec(GetMousePosition(), scoresButton.rect);
    collisionButton.isHighlighted = CheckCollisionPointRec(GetMousePosition(), collisionButton.rect);
    quitButton.isHighlighted = CheckCollisionPointRec(GetMousePosition(), quitButton.rect);
}

void Level::DisplayHighScores() {
    if (CheckCollisionPointRec(GetMousePosition(), retryButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Initialization();
        gameState = PLAY;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), backToMain_MenuButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = MAIN_MENU;
    }
}

void Level::WritePlayerScore() {

    Writename();

    if (CheckCollisionPointRec(GetMousePosition(), backToMain_MenuButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = MAIN_MENU;
    }
}

void Level::Collision() {

    CollisionUpdate();

    if (CheckCollisionPointRec(GetMousePosition(), backToMain_MenuButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = MAIN_MENU;
    }
}

void Level::GameOver() {
    if (CheckCollisionPointRec(GetMousePosition(), retryButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Initialization();
        gameState = PLAY;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), backToMain_MenuButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        gameState = MAIN_MENU;
    }
}



void Level::Quit() {
    De_Initialization();
    exit(0);
}

// Update collision detection  (also color changes when is done right).

void Level::CollisionUpdate() {

    // Change the color of the middle shape back to red

    midSquare.color = DARKGREEN;
    midCircle.color = DARKGREEN;
    midLine.color = DARKGREEN;

    // Check for mouse clicks to select shapes (circle, box, line).

    if (CircleCollision(midCircle, movingCircle)) {
        midCircle.color = YELLOW;

    }
    if (CircleCloneCollision(midCircle, movingSquare) || CircleCloneCollision(movingCircle, midSquare)) {
        midSquare.color = YELLOW;
    }
    if (CloneCollision(midSquare, movingSquare)) {
        midSquare.color = YELLOW;
    }

    if (CircleLineCollision((float)movingCircle.centerX, (float)movingCircle.centerY, movingCircle.radius, midLine.startPos.x, midLine.startPos.y, midLine.endPos.x, midLine.endPos.y)) {
        midLine.color = YELLOW;
    }

    if (CheckCollisionPointRec(GetMousePosition(), circleButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        shapes = CIRCLE;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), boxButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        shapes = SQUARE;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), lineButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        shapes = LINE;
    }


    if (CheckCollisionPointRec(GetMousePosition(), circleButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        shapes = CIRCLE;

        midCircle.appear = true;
        movingCircle.appear = true;
        midSquare.appear = false;
        midLine.appear = false;
        movingSquare.appear = false;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), boxButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        shapes = SQUARE;

        midSquare.appear = true;
        movingSquare.appear = true;
        movingCircle.appear = true;
        midCircle.appear = false;
        midLine.appear = false;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), lineButton.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        shapes = LINE;

        midLine.appear = true;
        movingCircle.appear = true;
        midSquare.appear = false;
        midCircle.appear = false;
        movingSquare.appear = false;
    }

    // Handle mouse - moving shapes interaction.

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointCircle(GetMousePosition(), { (float)movingCircle.centerX, (float)movingCircle.centerY }, movingCircle.radius)) {
            mouseOffset = { 0, 0 };

            movingCircle.centerX = GetMouseX() - (int)mouseOffset.x;
            movingCircle.centerY = GetMouseY() - (int)mouseOffset.y;
        }


        if (CheckCollisionPointRec(GetMousePosition(), { (float)movingSquare.posX, (float)movingSquare.posY, (float)movingSquare.width, (float)movingSquare.height })) {
            mouseOffset = { (float)movingSquare.width / 2, (float)movingSquare.height / 2 };

            movingSquare.posX = GetMouseX() - (int)mouseOffset.x;
            movingSquare.posY = GetMouseY() - (int)mouseOffset.y;
        }
    }
}



// Update the game screen (player movement, enemy spawns, collisions, and missiles)


void Level::UpdateMainGame() {

    // Calculate the time elapsed.


    double currentTime = GetTime();
    passedTime = currentTime - Time;

    // Automatic enemy spawn at a specific intervals.

    double elapsedTimeSinceLastSpawn = currentTime - lastEnemySpawnTime;
    if (elapsedTimeSinceLastSpawn >= enemySpawnInterval) {
        SpawnEnemyShip();
        SpawnFasterEnemyPlanes();
        lastEnemySpawnTime = currentTime;
    }

    // enemy planes collisions and movement.

    for (auto& enemy : enemyplanes) {
        if (!enemy.isDead) {
            Rectangle enemyHitbox = { enemy.position.x, enemy.position.y, (float)enemy.width, (float)enemy.height };

            if (CheckCollisionRecs(plane.rec, enemyHitbox)) {
                plane.isDead = true;

                if (IsHighScore(highscore.score)) {
                    enemyplanes.clear();
                    gameState = WRITE_NAME;
                }
                else {
                    enemyplanes.clear();
                    gameState = GAME_OVER;
                }
            }

            enemy.position.x -= enemy.speed;

            if (enemy.position.x < -static_cast<float>(enemy.width)) {
                enemy.isDead = true;
            }
        }
    }

    // indistructable enemy planes collisions and movement.

    for (auto& indestructibleEnemy : fasterEnemyPlanes) {
        if (!indestructibleEnemy.isDead) {
            Rectangle indestructibleEnemyHitbox = { indestructibleEnemy.position.x, indestructibleEnemy.position.y, indestructibleEnemy.width, indestructibleEnemy.height };
            if (CheckCollisionRecs(plane.rec, indestructibleEnemyHitbox)) {
                plane.isDead = true;

                if (IsHighScore(highscore.score)) {
                    fasterEnemyPlanes.clear();
                    gameState = WRITE_NAME;
                }
                else {
                    fasterEnemyPlanes.clear();
                    gameState = GAME_OVER;
                }
            }
            indestructibleEnemy.position.x -= indestructibleEnemy.speed;
            if (indestructibleEnemy.position.x < -static_cast<float>(indestructibleEnemy.width)) {
                indestructibleEnemy.isDead = true;
            }
        }
    }



    // Collision between enemies and missiles

    MissileEnemyCollisions();
}



//--------------- GAME UPDATES ------------------------------------------------------

// player movement with keyboard input.

void Level::UserMovementControl()
{
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        plane.rec.y -= plane.speed;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        plane.rec.y += plane.speed;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        plane.rec.x -= plane.speed; // Decrement the x position for left movement
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        plane.rec.x += plane.speed; // Increment the x position for right movement
    }

    if (plane.rec.x < 0)
    {
        plane.rec.x = 0;
    }
    else if (plane.rec.y < 0)
    {
        plane.rec.y = 0;
    }
    else if (plane.rec.y + plane.rec.height > screenHeight)
    {
        plane.rec.y = screenHeight - plane.rec.height;
    }
}

// Update the animation for player.

void Level::UpdatePlayerSprite() {

    currentFrame++;
    if (currentFrame >= NUM_plane_FRAMES) {
        currentFrame = 0;
    }
}

// Automatically shoot missiles.

void Level::ShootAutomatic() {
    static double lastShotTime = GetTime();

    double currentTime = GetTime();
    double elapsedTime = currentTime - lastShotTime;


    if (elapsedTime >= 0.5) {
        ShootMissiles();
        lastShotTime = currentTime;
    }
}

// Create a Missiles from the player's plane.

void Level::ShootMissiles() {

    Vector2 projVelocity = { projectileSpeed, 0.0f };
    Missiles proj({ plane.rec.x + plane.rec.width, plane.rec.y + plane.rec.height / 2 }, projVelocity);
    proj.isActive = true;
    missiles.push_back(proj);
}

// Update the positions missiles.

void Level::UpdateMissiles() {

    for (int i = 0; i < missiles.size(); i++) {
        if (missiles[i].isActive) {
            missiles[i].position.x += missiles[i].velocity.x;

            // Check if the Missiles are off-screen and deactivate it.

            if (missiles[i].position.x > screenWidth) {
                missiles[i].isActive = false;
            }
        }
    }
}

// Spawn an enemy plane with random initial position.

void Level::SpawnEnemyShip() {
    Enemyplane enemy;
    enemy.position = { static_cast<float>(screenWidth), static_cast<float>(GetRandomValue(0, screenHeight - enemy.height)) };
    enemy.speed = 10.0f;
    enemy.isDead = false;
    enemy.width = 50;
    enemy.height = 50;
    enemyplanes.push_back(enemy);
}

// Spawn an indestructable enemy plane with random initial position.

void Level::SpawnFasterEnemyPlanes()
{

    FasterEnemyPlanes InEnemy;
    InEnemy.position = { static_cast<float>(screenWidth), static_cast<float>(GetRandomValue(0, static_cast<int>(screenHeight) - static_cast<int>(InEnemy.height))) };
    InEnemy.speed = 20.0f;
    InEnemy.isDead = false;
    InEnemy.color = SKYBLUE;
    InEnemy.width = 40;
    InEnemy.height = 40;
    fasterEnemyPlanes.push_back(InEnemy);
}

// Handle collisions between missiles and enemy planes.

void Level::MissileEnemyCollisions() {
    for (int i = 0; i < missiles.size(); i++) {
        if (missiles[i].isActive) {
            for (int j = 0; j < enemyplanes.size(); j++) {
                if (!enemyplanes[j].isDead) {

                    // Check for collision between Missiles - enemy plane.

                    if (CheckCollisionRecs(
                        { missiles[i].position.x, missiles[i].position.y, missiles[i].size.x, missiles[i].size.y },
                        { enemyplanes[j].position.x, enemyplanes[j].position.y, (float)enemyplanes[j].width, (float)enemyplanes[j].height }
                    )) {

                        // Deactivate the Missiles, kill enemy plane and add score for each one.
                        PlaySound(hitSound);
                        missiles[i].isActive = false;
                        enemyplanes[j].isDead = true;
                        highscore.score += 50;
                    }
                }
            }
        }
    }
}


//--------------- Collisions Functions ------------------------------------------------------

// Check for collision between two circles.

bool Level::CircleCollision(const Circle& midCircle, const Circle& movingCircle) {

    // Calculate the distance between the centers of the two circles.

    float dx = static_cast<float>(movingCircle.centerX - midCircle.centerX);
    float dy = static_cast<float>(movingCircle.centerY - midCircle.centerY);
    float distance = sqrt(dx * dx + dy * dy);

    // Check if the distance is less than or equal to the sum of their radius.

    return distance <= (midCircle.radius + movingCircle.radius);
}

// Check for collision between a circle and a box(axis-aligned bounding).

bool Level::CircleCloneCollision(const Circle& circle, const Square& box) {

    // Find the closest point ( X, Y) on the Clone to the center of the circle.

    float closestX = (float)std::clamp(circle.centerX, box.posX, box.posX + box.width);
    float closestY = (float)std::clamp(circle.centerY, box.posY, box.posY + box.height);

    // Calculate the squared distance between the circle center and the closest point.

    float dx = circle.centerX - closestX;
    float dy = circle.centerY - closestY;
    float distanceSquared = (dx * dx) + (dy * dy);

    // Check if the squared distance is less than or equal to the squared radius of the circle.

    return distanceSquared <= (circle.radius * circle.radius);
}

// Check for collision between a circle and a line.

bool Level::CircleLineCollision(float centerX, float centerY, float circleRadius, float startPosX, float startPosY, float endPosX, float endPosY) const {

    // Calculate vectors AB and AC.

    const float ABX = endPosX - startPosX;
    const float ABY = endPosY - startPosY;

    // Calculate dot products for AB and AC vectors.

    float ACX = centerX - startPosX;
    float ACY = centerY - startPosY;

    // Calculate parameter t to find the closest point on the line.

    float dotACAB = (ACX * ABX + ACY * ABY);
    float dotABAB = (ABX * ABX + ABY * ABY);


    float t = dotACAB / dotABAB;

    if (t < 0) t = 0;
    if (t > 1) t = 1;

    // Calculate the closest point on the line

    float closestPointX = startPosX + ABX * t;
    float closestPointY = startPosY + ABY * t;

    // Calculate the distance between the circle center and the closest point.

    float dx = centerX - closestPointX;
    float dy = centerY - closestPointY;
    float distance = sqrt(dy * dy + dx * dx);

    // Check if the distance is less than or equal to the circle's radius.

    return (distance <= circleRadius);
}

// Check for collision between two boxes (axis-aligned bounding boxes)

bool Level::CloneCollision(const Square& box1, const Square& box2) {

    // Check if there is no overlap in the x-axis.

    if ((box1.posX + box1.width) < box2.posX || (box2.posX + box2.width) < box1.posX) {
        return false;
    }

    // Check if there is no overlap in the y-axis.

    if ((box1.posY + box1.height) < box2.posY || (box2.posY + box2.height) < box1.posY) {
        return false;
    }

    // If there is overlap in both axis, the boxes are colliding.

    return true;
}


//--------------- Name and Highscore Functions ------------------------------------------------------


// Write the highscores to a .txt file.

void Level::WriteHighScoreToFile()
{
    std::ofstream highScoreFile("./Source/Assets/HighScore.txt");

    if (highScoreFile.is_open()) {
        for (int i = 0; i < 5; i++) {
            highScoreFile << highScores[i].score << " " << highScores[i].name << "\n";
        }
        highScoreFile.close();
    }
}

// Handle the player's name input for high score entry.

void Level::Writename()
{
    if (!nameConfirmed) {
        // Check if the mouse is inside the text field
        if (CheckCollisionPointRec(GetMousePosition(), textSquare)) {
            mouseOnText = true;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0 && letterCount < 3) {
                name += static_cast<char>(key);
                letterCount++;
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                name.pop_back();
            }
        }
        else {
            // If the mouse is outside the text field, remove the cursor
            mouseOnText = false;
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Check if Enter is pressed and the name is complete
        if (IsKeyPressed(KEY_ENTER) && letterCount == 3) {
            nameConfirmed = true;

            HighScore newHighScore;
            newHighScore.score = highscore.score;
            newHighScore.name = name;

            highScores.push_back(newHighScore);

            std::sort(std::begin(highScores), std::end(highScores), [](const HighScore& a, const HighScore& b) {
                return a.score > b.score;
                });

            if (highScores.size() > 5) {
                highScores.pop_back();
            }

            WriteHighScoreToFile();

            gameState = HIGH_SCORE;
        }
    }
}

// Check if the player's current score is a high score.

bool Level::IsHighScore(int currentScore) {
    for (const HighScore& score : highScores) {
        if (currentScore >= score.score) {
            return true;
        }
    }
    return false;
}
// Compare function for sorting high scores in descending order.

bool Level::CompareHighScores(const HighScore& a, const HighScore& b) {
    return a.score > b.score;
}