#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "includes/MerlinClass.h"
#include "includes/MerlinMath.h"

//screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;


class Player : public KinematicBody {
public:
	Player() {
		position = { 0, 0 };
		size = { 1, 1 };
		color = WHITE;
		velocity = { 0, 0 };
	}

	Player(Vector2 position, Vector2 size, Color color, Vector2 velocity) {
		this->position = position;
		this->size = size;
		this->color = color;
		this->velocity = velocity;
	}

	void update(vector<GameObject*> gameObjects) override {
		if (IsKeyDown(KEY_W)) {
			velocity.y = -2;
		}
		else if (IsKeyDown(KEY_S)) {
			velocity.y = 2;
		}
		else {
			velocity.y = 0;
		}

		if (IsKeyDown(KEY_A)) {
			velocity.x = -2;
		}
		else if (IsKeyDown(KEY_D)) {
			velocity.x = 2;
		}
		else {
			velocity.x = 0;
		}

		move();

		//check for collisions
		processCollisions(gameObjects);
	}
};

class tile : public Collision {
	//tile class, a tile is spawned by the tilemap class and is a collision object.
	public:
    // Default constructor
    tile() {
        position = { 0, 0 };
        size = { 8, 8 };
        color = RED;
    }

    // Constructor with arguments
    tile(Vector2 position, Vector2 size, Color color) {
        this->position = position;
        this->size = size;
        this->color = color;

		printf("Tile created at %f, %f\n", position.x, position.y);
    }
};

class Tilemap : public GameObject {
	//Tilemap class, reads tilemap from a file and for each tile spawns a tile object.
	public:
	string tilemapPath;

	Tilemap(string tilemapPath) {
		this->tilemapPath = tilemapPath;
	}

	void spawnTiles(vector<GameObject*> gameObjects) {
		//spawn tiles
		//for each tile in the tilemap file, create a tile object and add it to the game objects array
		printf("Spawning tiles\n");

		//open tilemap file
		FILE* tilemapFile = fopen(tilemapPath.c_str(), "r");

		//check if file exists
		if (tilemapFile == NULL) {
			printf("Tilemap file not found\n");
			return;
		}

		//create tilemap array
		int tilemapWidth = 16;
		int tilemapHeight = 16;
		int tilemap[tilemapWidth][tilemapHeight];

		//read tilemap file to array
		for (int y = 0; y < tilemapHeight; y++) {
			for (int x = 0; x < tilemapWidth; x++) {
				fscanf(tilemapFile, "%d", &tilemap[x][y]);
			}
		}

		//close tilemap file
		fclose(tilemapFile);

		//spawn tiles
		//for each tile in the tilemap array, create a tile object and add it to the game objects array
		for (int y = 0; y < tilemapHeight; y++) {
			for (int x = 0; x < tilemapWidth; x++) {
				if (tilemap[x][y] == 1) {
					tile* newTile = new tile( { float(x) * 8, float(y) * 8}, { 8, 8 }, RED);
					gameObjects.push_back(newTile);
				}
			}
		}
	}

	void ready(vector<GameObject*> gameObjects) override {
		spawnTiles(gameObjects);
	}

};

int main() {
	//initialisation
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); //set window flags
	InitWindow(screenWidth, screenHeight, "Merlin Engine"); //init window
	RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight); //init render texture
	SetTargetFPS(30); // Set our game to run at 30 frames-per-second

	//init

	std::vector<GameObject*> gameObjects;//list of game objects to be processed


	//declaring all objects in the scene and adding them to the array of game objects
	Player* player = new Player({ 120, 64 }, { 8, 8 }, RED, {0, 0});
	gameObjects.push_back(player);
	 
	Tilemap* tilemap = new Tilemap("C:/Users/leons/Documents/projects/merlin/gameData/tilemap.txt");
	gameObjects.push_back(tilemap);

	//proccess game objects ready function
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->ready(gameObjects);
	}

	while (!WindowShouldClose())

	{
		//process all game objects
		for (GameObject* gameObject : gameObjects)
		{
			gameObject->update(gameObjects);
		}

		//drawing

		BeginTextureMode(target); //begin drawing to render texture

			ClearBackground(WHITE); //clear render texture

			//draw all objects that are, or inherit from visual instance class
			for (GameObject* gameObject : gameObjects)
			{
    			// Check if the object is visible and is a visual instance or a subclass of visual instance
				VisualInstance* visualInstance = dynamic_cast<VisualInstance*>(gameObject);
				if (visualInstance != nullptr && visualInstance->getVisible())
				{
					visualInstance->draw();
				}
			}

		EndTextureMode(); //end drawing to render texture

		//draw render texture to screen
		BeginDrawing(); //begin drawing to screen
		ClearBackground(BLACK); //clear screen
		DrawTexturePro(target.texture,
			Rectangle{ 0, 0, float(target.texture.width), float(-target.texture.height) },
			Rectangle{ 0, 0, float(GetScreenWidth()), float(GetScreenHeight()) },
			Vector2{ 0, 0 },
			0.0f,
			WHITE); //draw render texture to screen

		DrawFPS(10, 10); //draw fps counter

		EndDrawing(); //end drawing to screen
	}

	//deinitialisation
	CloseWindow(); //close window and openGL context
	return 0;
}
