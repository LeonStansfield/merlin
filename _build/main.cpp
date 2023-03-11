#include <math.h>
#include <vector>
#include <iostream>

#include "raylib.h"
#include "VisualInstance.h"
#include "collision.h"
#include "KinematicBody.h"

//screen width and height variables
const int screenWidth = 896;
const int screenHeight = 896;
const int gameScreenWidth = 128;
const int gameScreenHeight = 128;


int main() {
	//initialisation
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); //set window flags
	InitWindow(screenWidth, screenHeight, "Merlin Engine"); //init window
	RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight); //init render texture
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	//init
	KinematicBody player = KinematicBody(Vector2 {0, 0}, Vector2 {16, 16}, BLACK, Vector2 {0, 0});

	while (!WindowShouldClose())
	{
		//update
	

		//drawing

		BeginTextureMode(target); //begin drawing to render texture

			ClearBackground(WHITE); //clear render texture
			player.draw(); //draw player

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

			DrawFPS(10, 10); //draw fps counter

		EndDrawing(); //end drawing to screen
	}

	//deinitialisation
	CloseWindow(); //close window and openGL context
	return 0;
}