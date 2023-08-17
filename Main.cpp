#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "GlobalVariables.h"
#include "MerlinClass.h"
#include "player.h"

// --- TODO ---
// - Use the new globals script to upgrade camera movement to not be so silly.
// - redo / improve collision resolution (currently it is silly)
// - Animated sprites

// screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;

void ready(std::vector<GameObject*>& gameObjects)
{
	// declaring all objects in the scene and adding them to the array of game objects

	Tilemap tilemap = Tilemap("gameData/tilemap.txt", 8);
	tilemap.spawnTiles(gameObjects);

	// proccess game objects ready function
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->ready(gameObjects);
	}

	int i = 0;
	for (GameObject* gameObject : gameObjects) 
	{
		i += 1;
		if (dynamic_cast<Player*>(gameObject) != nullptr) {
			break;
		}

	}
}

void update(std::vector<GameObject*>& gameObjects)
{
	// process all game objects
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->update(gameObjects);
	}

	// Determine the current room
	if (GlobalVariables::GetInstance().playerReference != -1)
	{
		GlobalVariables::GetInstance().playerPosition = gameObjects[GlobalVariables::GetInstance().playerReference]->getPosition();
	}
	else
	{
		GlobalVariables::GetInstance().playerPosition = { 0, 0 };
	}
	int currentRoomX = GlobalVariables::GetInstance().playerPosition.x / gameScreenWidth;
	int currentRoomY = GlobalVariables::GetInstance().playerPosition.y / gameScreenHeight;
	// Set the camera offset
	GlobalVariables::GetInstance().cameraOffset = { float(currentRoomX * 128), float(currentRoomY * 128) };
}

void draw(std::vector<GameObject*>& gameObjects)
{
	int renderLayer = 0;
	// For each render layer
	for (renderLayer = 0; renderLayer < 10; renderLayer++) {
		// for all objects that are, or inherit from visual instance class
		for (GameObject* gameObject : gameObjects)
		{
			// determine what room the object is in
			int objectRoomX = gameObject->getPosition().x / gameScreenWidth;
			int objectRoomY = gameObject->getPosition().y / gameScreenHeight;
			bool isInCurrentRoom = true;
			if ((GlobalVariables::GetInstance().cameraOffset.x / 128) != objectRoomX || (GlobalVariables::GetInstance().cameraOffset.y / 128) != objectRoomY)
			{
				isInCurrentRoom = false;
			}

			// Check if the object is visible, is in the current render layer and is a visual instance or a subclass of visual instance
			VisualInstance* visualInstance = dynamic_cast<VisualInstance*>(gameObject);
			if (visualInstance != nullptr && visualInstance->getVisible() && visualInstance->getRenderLayer() == renderLayer && isInCurrentRoom)
			{
				visualInstance->draw(GlobalVariables::GetInstance().cameraOffset);
			}
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
		ClearBackground(WHITE); // clear render texture
		draw(gameObjects); // draw game objects
		EndTextureMode(); // end drawing to render texture

		// draw render texture to screen
		BeginDrawing();	// begin drawing to screen
		ClearBackground(BLACK); // clear screen
		DrawTexturePro(target.texture,
			Rectangle{ 0, 0, float(target.texture.width), float(-target.texture.height) },
			Rectangle{ 0, 0, float(GetScreenWidth()), float(GetScreenHeight()) },
			Vector2{ 0, 0 },
			0.0f,
			WHITE);	// draw render texture to screen
		DrawFPS(10, 10); // draw fps counter
		EndDrawing(); // end drawing to screen
	}

	// deinitialisation
	end(gameObjects);
	CloseWindow(); // close window and openGL context
	return 0;
}