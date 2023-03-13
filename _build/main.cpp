#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

#include "raylib.h"

//screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;

class GameObject {
public:
	Vector2 position;

	GameObject() {
		position = { 0, 0 };
	}

	GameObject(Vector2 position) {
		this->position = position;
	}

	Vector2 getPosition() {
		return position;
	}

	void setPosition(Vector2 position) {
		this->position = position;
	}

	virtual void update() {
	}
};

class VisualInstance : public GameObject {
public:
	Vector2 size;
	Color color;

	VisualInstance() {
		position = { 0.0, 0.0 };
		size = { 1, 1 };
		color = WHITE;
	}

	VisualInstance(Vector2 position, Vector2 size, Color color) {
		this->position = position;
		this->size = size;
		this->color = color;
	}

	Vector2 getSize() {
		return size;
	}

	void setSize(Vector2 size) {
		this->size = size;
	}

	Color getColor() {
		return color;
	}

	void setColor(Color color) {
		this->color = color;
	}

	void draw() {
		DrawRectangle(position.x, position.y, size.x, size.y, color);
	}
};

class Collision : public VisualInstance {
public:
	Collision() {
		position = { 0.0, 0.0 };
		size = { 1, 1 };
		color = MAROON;
	}

	Collision(Vector2 position, Vector2 size, Color color) {
		this->position = position;
		this->size = size;
		this->color = color;
	}

	bool checkCollision(Collision other) {
		return CheckCollisionRecs({ position.x, position.y, size.x, size.y }, { other.position.x, other.position.y, other.size.x, other.size.y });
	}

	void drawCollisionBox() {
		DrawRectangleLines(position.x, position.y, size.x, size.y, color);
	}
};

class KinematicBody : public Collision {
public:
	Vector2 velocity;

	KinematicBody() {
		position = { 0, 0 };
		size = { 1, 1 };
		color = WHITE;
		velocity = { 0, 0 };
	}

	KinematicBody(Vector2 position, Vector2 size, Color color, Vector2 velocity) {
		this->position = position;
		this->size = size;
		this->color = color;
		this->velocity = velocity;
	}

	Vector2 getVelocity() {
		return velocity;
	}

	void setVelocity(Vector2 velocity) {
		this->velocity = velocity;
	}

	void move() {
		position.x += velocity.x;
		position.y += velocity.y;
	}
};

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

	std::vector<GameObject*> gameObjects;//list of game objects to be processed
	std::vector<VisualInstance*> visualInstances;//list of visual instances to be drawn
	std::vector<Collision*> collisionObjects;//list of collisionObjects to be checked
	std::vector<KinematicBody*> kinematicBodies;//list of kinematic bodies to be moved


	Player* player = new Player({ 120, 72 }, { 8, 8 }, RED, {0, 0}); //create player
	gameObjects.push_back(player);
	visualInstances.push_back(player);
	collisionObjects.push_back(player);
	kinematicBodies.push_back(player);
	 
	Collision* wall = new Collision({ 16, 16 }, { 64, 64 }, BLUE);
	gameObjects.push_back(wall);
	visualInstances.push_back(wall);
	collisionObjects.push_back(wall);

	while (!WindowShouldClose())

	{
		//process
		for (GameObject* gameObject : gameObjects)
		{
			gameObject->update();
		}

		//for kinematic body, check if it is colliding with a collision object.
		for (KinematicBody* kinematicBody : kinematicBodies) {
			for (Collision* collisionObject : collisionObjects) {
				if (kinematicBody->checkCollision(*collisionObject)) {
					kinematicBody->setVelocity({ 0, 0 });
					kinematicBody->setColor(RED);
				}
				else {
					kinematicBody->setColor(GREEN);
				}
			}
		}

		//drawing

		BeginTextureMode(target); //begin drawing to render texture

			ClearBackground(WHITE); //clear render texture

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
