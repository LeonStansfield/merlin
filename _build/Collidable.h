#pragma once

#include "raylib.h"
#include "Object.h"

class Collidable : protected Object {
protected:
	Vector2 size;
	Color debugColor;
public:
	Collidable() {
		size = { 0, 0 };
		debugColor = ORANGE;
	}

	Collidable(Vector2 position, Vector2 size) {
		this->position = position;
		this->size = size;
	}

	Vector2 getSize() {
		return size;
	}

	void setSize(Vector2 size) {
		this->size = size;
	}
};