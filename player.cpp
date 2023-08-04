#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "player.h"


Player::Player()
	{
		position = { 0, 0 };
		size = { 1, 1 };
		layer = 1;
		hasTexture = false;
		color = WHITE;
		texturePath = "";
		velocity = { 0, 0 };
	}

Player::Player(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath, Vector2 velocity)
	{
		this->position = position;
		this->size = size;
		this->hasTexture = hasTexture;
		this->color = color;
		this->texturePath = texturePath;
		this->velocity = velocity;
		this->layer = layer;
	}

	void Player::update(vector<GameObject*> gameObjects)
	{
		// check if the player is grounded
		if (pget({ position.x + size.x / 2, position.y + size.y + 1 }, 1, gameObjects))
		{
			isGrounded = true;
		}
		else
		{
			isGrounded = false;
		}

		// check if player is on a roof
		if (pget({ position.x + size.x / 2, position.y - 1 }, 1, gameObjects))
		{
			velocity.y = 0;
		}

		// check if player is on a wall
		if (pget({ position.x + size.x + 1, position.y + size.y / 2 }, 1, gameObjects))
		{
			velocity.x = 0;
		}
		else if (pget({ position.x - 1, position.y + size.y / 2 }, 1, gameObjects))
		{
			velocity.x = 0;
		}

		if (!isGrounded)
		{
			if (velocity.y <= -0.1)
			{
				velocity.y += gravity;
			}
			else if (velocity.y > -0.1)
			{
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

		if (velocity.y > 8)
		{
			velocity.y = 8;
		}
		else if (velocity.y < -8)
		{
			velocity.y = -8;
		}

		if (IsKeyDown(KEY_SPACE) && isGrounded)
		{
			velocity.y = jumpVelocity;
		}

		move();

		// check for collisions
		processCollisions(gameObjects);
	}