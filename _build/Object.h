#pragma once

#include "raylib.h"

class Object {
protected:
	Vector2 position;
public:
	Object() {
		position = { 0, 0 };
	}

	Object(Vector2 position) {
		this->position = position;
	}

	Vector2 getPosition() {
		return position;
	}

	void setPosition(Vector2 position) {
		this->position = position;
	}
};