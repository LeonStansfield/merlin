#pragma once

#include "raylib.h"
#include "VisualInstance.h"

class Collision: public VisualInstance {
public:
	Collision() {
		position = { 0, 0 };
		size = { 1, 1 };
		color = WHITE;
	}

	Collision(Vector2 position, Vector2 size, Color color) {
		this->position = position;
		this->size = size;
		this->color = color;
	}

	void drawCollisionBox() {
		DrawRectangleLines(position.x, position.y, size.x, size.y, color);
	}
};