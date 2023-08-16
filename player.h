#pragma once

#include <vector>
#include <fstream>

#include "raylib.h"
#include "MerlinClass.h"

class Player : public KinematicBody
{
private:
	bool isGrounded = false;
	float gravity = 0.15;
	float fallMultiplier = 2;
	float jumpVelocity = -3;

public:
	Player();

	Player(Vector2 position, Vector2 size, int layer, string texturePath);

	void update(vector<GameObject*> gameObjects) override;
};
