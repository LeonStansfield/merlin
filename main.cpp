#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "includes/MerlinClass.h"
#include "includes/MerlinMath.h"

// screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;

bool pget(Vector2 pos, int layer, const std::vector<GameObject*>& gameObjects) {
    for (GameObject* gameObject : gameObjects) {
        Collision* collision = dynamic_cast<Collision*>(gameObject);
        if (collision != nullptr && collision->getLayer() == layer) {
            if (CheckCollisionPointRec(pos, { collision->position.x, collision->position.y, collision->size.x, collision->size.y })) {
                return true;
            }
        }
    }
    return false; // no collision found with the specified layer
}


class Player : public KinematicBody
{
private:
	bool isGrounded = false;
	float gravity = 0.15;
	float fallMultiplier = 2;
	float jumpVelocity = -3;

public:
	Player()
	{
		position = {0, 0};
		size = {1, 1};
		layer = 1;
		color = WHITE;
		velocity = {0, 0};
	}

	Player(Vector2 position, Vector2 size, int layer, Color color, Vector2 velocity)
	{
		this->position = position;
		this->size = size;
		this->color = color;
		this->velocity = velocity;
		this->layer = layer;
	}

	void update(vector<GameObject *> gameObjects) override
	{
		//check if the player is grounded
		if (pget({ position.x + size.x / 2, position.y + size.y + 1 }, 1, gameObjects))
		{
			isGrounded = true;
		}
		else
		{
			isGrounded = false;
		}

		//check if player is on a roof
		if (pget({ position.x + size.x / 2, position.y - 1 }, 1, gameObjects))
		{
			velocity.y = 0;
		}

		//check if player is on a wall
		if (pget({ position.x + size.x + 1, position.y + size.y / 2 }, 1, gameObjects))
		{
			velocity.x = 0;
		}
		else if (pget({ position.x - 1, position.y + size.y / 2 }, 1, gameObjects))
		{
			velocity.x = 0;
		}

		if (!isGrounded) {
			if (velocity.y <= -0.1) {
				velocity.y += gravity;
			}
			else if (velocity.y > -0.1) {
				velocity.y += gravity * fallMultiplier;
			}
		}
		else
		{
			velocity.y = 0;
		}

		// check for input
		if (IsKeyDown(KEY_A))
		{
			velocity.x -= 0.3;
		}
		else if (IsKeyDown(KEY_D))
		{
			velocity.x += 0.3;
		}
		else
		{
			if (velocity.x > 0.1)
			{
				velocity.x -= 0.3;
			}
			else if (velocity.x < -0.1)
			{
				velocity.x += 0.3;
			}
			else
			{
				velocity.x = 0;
			}
		}

		if (velocity.x > 2)
		{
			velocity.x = 2;
		}
		else if (velocity.x < -2)
		{
			velocity.x = -2;
		}

		if (IsKeyDown(KEY_SPACE) && isGrounded)
		{
			velocity.y = jumpVelocity;
		}

		move();

		// check for collisions
		processCollisions(gameObjects);
	}
};

void ready(std::vector<GameObject*>& gameObjects){
	// declaring all objects in the scene and adding them to the array of game objectsd
	Player *player = new Player({64, 64}, {8, 8}, 1, BLUE, {0, 0});
	gameObjects.push_back(player);

	Tilemap *tilemap = new Tilemap("gameData/tilemap.txt", 8);
	gameObjects.push_back(tilemap);

	// proccess game objects ready function
	for (GameObject *gameObject : gameObjects)
	{
		gameObject->ready(gameObjects);
	}
}

void update(std::vector<GameObject*>& gameObjects, Player& player, Camera2D& camera){
	// process all game objects
	for (GameObject *gameObject : gameObjects)
	{
		gameObject->update(gameObjects);
	}

	// Determine the current room
	int currentRoomX = player.position.x / 128;
	int currentRoomY = player.position.y / 128;

	// Calculate the center point of the room
	float cameraX = currentRoomX * screenHeight + (screenHeight / 2);
	float cameraY = currentRoomY * screenHeight + (screenHeight / 2);

	// Clamp the camera position to the room bounds
	camera.target = Vector2{ float(cameraX), float(cameraY)};

}

void draw(std::vector<GameObject*>& gameObjects){
	// draw all objects that are, or inherit from visual instance class
	for (GameObject *gameObject : gameObjects)
	{
		// Check if the object is visible and is a visual instance or a subclass of visual instance
		VisualInstance *visualInstance = dynamic_cast<VisualInstance *>(gameObject);
		if (visualInstance != nullptr && visualInstance->getVisible())
		{
			visualInstance->draw();
		}

	}
}

int main()
{
	// initialisation
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);					   // set window flags
	InitWindow(screenWidth, screenHeight, "Merlin Engine");						   // init window
	RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight); // init render texture
	SetTargetFPS(30);															   // Set our game to run at 30 frames-per-second

	Camera2D camera = { 0 };
    camera.target = (Vector2){ 896.0f / 2, 896.0f / 2};
    camera.offset = (Vector2){ 896/2.0f, 896/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

	std::vector<GameObject *> gameObjects; // list of game objects to be processed
	
	ready(gameObjects);

	while (!WindowShouldClose())

	{
		// update
		update(gameObjects, *dynamic_cast<Player *>(gameObjects[0]), camera);

		// drawing
		BeginTextureMode(target); // begin drawing to render texture
			ClearBackground(WHITE); // clear render texture
			draw(gameObjects); // draw game objects
		EndTextureMode(); // end drawing to render texture

		// draw render texture to screen
		BeginDrawing();	// begin drawing to screen
			ClearBackground(BLACK); // clear screen
			BeginMode2D(camera); // begin 2d mode
				DrawTexturePro(target.texture,
							Rectangle{0, 0, float(target.texture.width), float(-target.texture.height)},
							Rectangle{0, 0, float(GetScreenWidth()), float(GetScreenHeight())},
							Vector2{0, 0},
							0.0f,
							WHITE); // draw render texture to screen
			EndMode2D(); // end 2d mode
			DrawFPS(10, 10); // draw fps counter
		EndDrawing(); // end drawing to screen
	}

	// deinitialisation
	CloseWindow(); // close window and openGL context
	return 0;
}