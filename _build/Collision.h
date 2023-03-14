#pragma once

#include "raylib.h"
#include "GameObject.h"
#include "VisualInstance.h"

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