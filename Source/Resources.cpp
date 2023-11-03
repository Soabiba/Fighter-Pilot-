#include "raylib.h"
#include "Level.h"

void Level::Resources()
{
	//---------------Textures------------------------------------------------
	

	//Upload textures for Game

	backgroundImage = LoadTexture("./Source/Assets/Background_Asset.png");
	planeImage = LoadTexture("./Source/Assets/Boost.png");
	menuImage = LoadTexture("./Source/Assets/menu.png");
	collisionBg = LoadTexture("./Source/Assets/menu.png");
	highscoreImage = LoadTexture("./Source/Assets/highscoreImage.png");
	//Upload sounds for Game

	hitSound = LoadSound("./Source/Assets/Hitting.mp3");
	gameOverSound = LoadSound("./Source/Assets/gameOver.mp3");
	hoveringSound = LoadSound("./Source/Assets/hovering sound.mp3");
	
}