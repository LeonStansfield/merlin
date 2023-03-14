#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "GameObject.h"
#include "VisualInstance.h"
#include "Collision.h"
#include "KinematicBody.h"

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

	void update() override {
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
	std::vector<VisualInstance*> visualInstances;//list of visual instances to be drawn
	std::vector<Collision*> collisionObjects;//list of collisionObjects to be checked
	std::vector<KinematicBody*> kinematicBodies;//list of kinematic bodies to be moved


	//declaring all objects in the scene
	Player* player = new Player({ 120, 64 }, { 8, 8 }, RED, {0, 0}); //create player
	gameObjects.push_back(player);
	visualInstances.push_back(player);
	collisionObjects.push_back(player);
	kinematicBodies.push_back(player);
	 
	Collision* wall = new Collision({ 32, 32 }, { 32, 32 }, BLUE);
	gameObjects.push_back(wall);
	visualInstances.push_back(wall);
	collisionObjects.push_back(wall);

	Collision* wall2 = new Collision({ 72, 96 }, { 48, 24 }, BLUE);
	gameObjects.push_back(wall2);
	visualInstances.push_back(wall2);
	collisionObjects.push_back(wall2);

	while (!WindowShouldClose())

	{
		//process all game objects
		for (GameObject* gameObject : gameObjects)
		{
			gameObject->update();
		}

		//for kinematic body, check if it is colliding with a collision object.
		for (KinematicBody* kinematicBody : kinematicBodies) {
			for (Collision* collisionObject : collisionObjects) {
				if (collisionObject == kinematicBody){
					continue;
				}
				if (kinematicBody->checkCollision(*collisionObject)) {
					kinematicBody->setColor(RED);
					//kinematicBody->resolveCollision(*collisionObject);
					break;
				}
				else {
					kinematicBody->setColor(GREEN);
				}
			}
		}

		//drawing

		BeginTextureMode(target); //begin drawing to render texture

			ClearBackground(WHITE); //clear render texture

			//draw all objects
			for (VisualInstance* visualInstance : visualInstances) {
				visualInstance->draw();
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
