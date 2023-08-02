#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "MerlinResources.h"
#include "MerlinMath.h"
#include "MerlinClass.h"
#include "Player.h"

// screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;
Vector2 cameraOffset = { 0, 0 };
Vector2 playerPosition = { 0, 0 };
int playerReference;
int scene = 1;

SceneManager sceneManager;

void ready(std::vector<GameObject*>& gameObjects)
{
	// declaring all objects in the scene and adding them to the array of game objectsd

	/* */
	Player* player = new Player({ 64, 64 }, { 6, 8 }, 1, true, white, "gameData/Textures/player.png", { 0, 0 });
	gameObjects.push_back(player);
	playerReference = gameObjects.size() - 1;

	sceneManager = SceneManager();
	sceneManager.loadScene(gameObjects, "gameData/scenes/sceneOne.msd");

	// proccess game objects ready function
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->ready(gameObjects);
	}
}

void update(std::vector<GameObject*>& gameObjects)
{
	// process all game objects
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->update(gameObjects);
	}

	string sceneOne = "gameData/scenes/sceneOne.msd";
	string sceneTwo = "gameData/scenes/sceneTwo.msd";
	if (IsKeyPressed(KEY_E))
	{
		if (scene == 1)
		{
			// change scene to sceneTwo
			sceneManager.changeScene(gameObjects, sceneTwo);
			scene = 2;
		}
		else if (scene == 2)
		{
			// change scene to sceneOne
			sceneManager.changeScene(gameObjects, sceneOne);
			scene = 1;
		}
	}

	// Determine the current room
	if (playerReference != -1)
	{
		playerPosition = gameObjects[playerReference]->getPosition();
	}
	else
	{
		playerPosition = { 0, 0 };
	}
	int currentRoomX = playerPosition.x / gameScreenWidth;
	int currentRoomY = playerPosition.y / gameScreenHeight;
	// Set the camera offset
	cameraOffset = { float(currentRoomX * 128), float(currentRoomY * 128) };
}

void draw(std::vector<GameObject*>& gameObjects)
{
	// draw all objects that are, or inherit from visual instance class
	for (GameObject* gameObject : gameObjects)
	{
		// Check if the object is visible and is a visual instance or a subclass of visual instance
		VisualInstance* visualInstance = dynamic_cast<VisualInstance*>(gameObject);
		if (visualInstance != nullptr && visualInstance->getVisible())
		{
			visualInstance->draw(cameraOffset);
		}
	}
}

void end(std::vector<GameObject*>& gameObjects)
{
	// draw all objects that are, or inherit from visual instance class
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->end();
	}
}

int main()
{
	// initialisation
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);					   // set window flags
	InitWindow(screenWidth, screenHeight, "Merlin Engine");						   // init window
	RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight); // init render texture
	SetTargetFPS(30);															   // Set our game to run at 30 frames-per-second

	Image icon = LoadImage("gameData/Textures/Icon/Icon_lrg.png"); // load icon
	SetWindowIcon(icon);

	std::vector<GameObject*> gameObjects; // list of game objects to be processed

	ready(gameObjects);

	while (!WindowShouldClose())

	{
		// update
		update(gameObjects);

		// drawing
		BeginTextureMode(target); // begin drawing to render texture
		ClearBackground(white);	  // clear render texture
		draw(gameObjects);		  // draw game objects
		EndTextureMode();		  // end drawing to render texture

		// draw render texture to screen
		BeginDrawing();			// begin drawing to screen
		ClearBackground(black); // clear screen
		DrawTexturePro(target.texture,
			Rectangle{ 0, 0, float(target.texture.width), float(-target.texture.height) },
			Rectangle{ 0, 0, float(GetScreenWidth()), float(GetScreenHeight()) },
			Vector2{ 0, 0 },
			0.0f,
			WHITE); // draw render texture to screen
		DrawFPS(10, 10);	   // draw fps counter
		EndDrawing();		   // end drawing to screen
	}

	// deinitialisation
	end(gameObjects);
	CloseWindow(); // close window and openGL context
	return 0;
}