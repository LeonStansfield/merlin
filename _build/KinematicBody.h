#pragma once

#include "raylib.h"
#include "Collision.h"

class KinematicBody : public Collision {
protected:
	Vector2 velocity;
public:
	KinematicBody() {
		position = { 0, 0 };
		size = { 1, 1 };
		color = WHITE;
		velocity = { 0, 0 };
	}

	KinematicBody(Vector2 position, Vector2 size, Color color, Vector2 velocity) {
		this->position = position;
		this->size = size;
		this->color = color;
		this->velocity = velocity;
	}

	Vector2 getVelocity() {
		return velocity;
	}

	void setVelocity(Vector2 velocity) {
		this->velocity = velocity;
	}

	void move() {
		position.x += velocity.x;
		position.y += velocity.y;
	}
};