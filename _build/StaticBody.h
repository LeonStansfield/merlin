#pragma once

#include "raylib.h"
#include "Collidable.h"

class StaticBody : protected Collidable {
public:
	StaticBody() {
			size = { 0, 0 };
		}

	StaticBody(Vector2 position, Vector2 size) {
			this->position = position;
			this->size = size;
		}

	void drawCollisionWireframe() {
		DrawRectangleLines(this->position.x, this->position.y, this->size.x, this->size.y, debugColor);
	}

	bool checkCollissions() {
		return false;
	}
};