#include <math.h>
#include <vector>
#include <iostream>

#include "raylib.h"

const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;


class Object {
private:
	Vector2 position;
	Vector2 size;
	Color color;
public:
	Object(Vector2 position, Vector2 size, Color color) {
		this->position = position;
		this->size = size;
		this->color = color;
	}

	Vector2 getPosition() {
		return position;
	}

	Vector2 getSize() {
		return size;
	}

	Color getColor() {
		return color;
	}

	void setPosition(Vector2 position) {
		this->position = position;
	}

	void setColor(Color color) {
		this->color = color;
	}

	void setSize(Vector2 size) {
		this->size = size;
	}

	void draw() {
		DrawRectangle(this->position.x, this->position.y, this->size.x, this->size.y, this->color);
	}
};

int main() {
	//initialisation
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "Cellular automata");
	RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	//init
	
	Object square(Vector2 {36, 36}, Vector2 {16, 16}, BLACK);

	while (!WindowShouldClose())
	{
		//update
		

		//drawing

		BeginTextureMode(target); //begin drawing to render texture

		ClearBackground(RAYWHITE); //clear render texture
		square.draw();

		EndTextureMode(); //end drawing to render texture

		//draw render texture to screen
		BeginDrawing(); //begin drawing to screen
		ClearBackground(BLACK); //clear screen
		DrawTexturePro(target.texture,
			Rectangle {0, 0, float(target.texture.width), float(-target.texture.height)},
			Rectangle {0, 0, float(GetScreenWidth()), float(GetScreenHeight())},
			Vector2 {0, 0},
			0.0f,
			WHITE); //draw render texture to screen
		EndDrawing(); //end drawing to screen
	}

	//deinitialisation
	CloseWindow(); //close window and openGL context
	return 0;
}