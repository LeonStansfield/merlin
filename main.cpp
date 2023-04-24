#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "includes/MerlinResources.h"
#include "includes/MerlinMath.h"
#include "includes/MerlinClass.h"
#include "includes/Player.h"

// screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;
Vector2 cameraOffset = {0, 0};
int playerReference = 0;
Vector2 playerPosition = {0, 0};


void ready(std::vector<GameObject*>& gameObjects){
	// declaring all objects in the scene and adding them to the array of game objectsd

	
	Scene *scene = new Scene("gameData/scenes/test-scene.msd");
	scene->load(gameObjects);
	

	/* */
	Player *player = new Player({64, 64}, {6, 8}, 1, true, white, "gameData/Textures/player.png", {0, 0});
	gameObjects.push_back(player);
	playerReference = gameObjects.size() - 1;
	Tilemap *tilemap = new Tilemap("gameData/tilemap.txt", 8);
	gameObjects.push_back(tilemap);
	/* */

	// proccess game objects ready function
	for (GameObject *gameObject : gameObjects)
	{
		gameObject->ready(gameObjects);
	}
}

void update(std::vector<GameObject*>& gameObjects){
	// process all game objects
	for (GameObject *gameObject : gameObjects)
	{
		gameObject->update(gameObjects);
	}

	// Determine the current room
	playerPosition = gameObjects[playerReference]->getPosition(); // player must always be the first object in the game objects array
	int currentRoomX = playerPosition.x / gameScreenWidth; 
	int currentRoomY = playerPosition.y / gameScreenHeight;
	// Set the camera offset
	cameraOffset = {float(currentRoomX * 128), float(currentRoomY * 128)};

}

void draw(std::vector<GameObject*>& gameObjects){
	// draw all objects that are, or inherit from visual instance class
	for (GameObject *gameObject : gameObjects)
	{
		// Check if the object is visible and is a visual instance or a subclass of visual instance
		VisualInstance *visualInstance = dynamic_cast<VisualInstance *>(gameObject);
		if (visualInstance != nullptr && visualInstance->getVisible())
		{
			visualInstance->draw(cameraOffset);
		}

	}
}

void end(std::vector<GameObject*>& gameObjects){
	// draw all objects that are, or inherit from visual instance class
	for (GameObject *gameObject : gameObjects)
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

	std::vector<GameObject *> gameObjects; // list of game objects to be processed
	
	ready(gameObjects);

	while (!WindowShouldClose())

	{
		// update
		update(gameObjects);

		// drawing
		BeginTextureMode(target); // begin drawing to render texture
			ClearBackground(white); // clear render texture
			draw(gameObjects); // draw game objects
		EndTextureMode(); // end drawing to render texture

		// draw render texture to screen
		BeginDrawing();	// begin drawing to screen
			ClearBackground(black); // clear screen
				DrawTexturePro(target.texture,
						Rectangle{0, 0, float(target.texture.width), float(-target.texture.height)},
						Rectangle{0, 0, float(GetScreenWidth()), float(GetScreenHeight())},
						Vector2{0, 0},
						0.0f,
						WHITE); // draw render texture to screen
			DrawFPS(10, 10); // draw fps counter
		EndDrawing(); // end drawing to screen
	}

	// deinitialisation
	end(gameObjects);
	CloseWindow(); // close window and openGL context
	return 0;
}