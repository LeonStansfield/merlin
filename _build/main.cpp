#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "VisualInstance.h"
#include "KinematicBody.h"

//screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;

class Player : public KinematicBody {
public:
	Player() {
		position = { 0.0, 0.0 };
		size = { 16, 16 };
		color = WHITE;
		velocity = { 0, 0 };
	}

	Player(Vector2 position, Vector2 size, Color color, Vector2 velocity) {
		this->position = position;
		this->size = size;
		this->color = color;
		this->velocity = velocity;
	}

	void update() {
		//update velocity
		if (IsKeyDown(KEY_W)) {
			velocity.y = -1;
		}
		else if (IsKeyDown(KEY_S)) {
			velocity.y = 1;
		}
		else {
			velocity.y = 0;
		}

		if (IsKeyDown(KEY_A)) {
			velocity.x = -1;
		}
		else if (IsKeyDown(KEY_D)) {
			velocity.x = 1;
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
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	//init

	Player player = Player(Vector2{ 0,  0 }, Vector2{ 16,  16 }, BLACK, Vector2{ 0,  0 });

	Collision box = Collision(Vector2{ 32,  32 }, Vector2{ 16,  16 }, RED);
	Collision box2 = Collision(Vector2{ 48,  48 }, Vector2{ 16,  16 }, RED);
	Collision box3 = Collision(Vector2{ 16,  48 }, Vector2{ 16,  16 }, RED);

	while (!WindowShouldClose())
	{
		//update

		player.update();

		// WORKING ON COLLISIONS
		/*
			Does the player collide with each box (Collision object, as in does the player have a method that checks collisions with all kinematic and collision bodies)
			Or does each box have a method that checks collisions with the player (KinematicBody object)

			The latter may be more performant as it only checks collisions with kinematic bodies, but the former may be more flexible as it can check collisions with any kinematic or collision body.
			This may also be confusing as I would have to act on the kinematic bodies in the collision class, which would be a bit weird and backwards.
		*/
		box.update(player);
		box2.update(player);
		box3.update(player);

		//drawing

		BeginTextureMode(target); //begin drawing to render texture

		ClearBackground(WHITE); //clear render texture
		player.draw();
		box.draw();
		box2.draw();
		box3.draw();

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
