#pragma once

#include "raylib.h"

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