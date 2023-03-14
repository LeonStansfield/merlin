#pragma once

#include "raylib.h"
#include "Collision.h"

class KinematicBody : public Collision {
public:
	Vector2 velocity;

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

	//resolve collision
	void resolveCollision(Collision& other) {

		// Get the distance between the centers of the two objects
		Vector2 distance = other.position - position;

		// Get the minimum distance to move to resolve the collision
		Vector2 minDistance = (other.size + size) * 0.5 - Vector2(abs(distance.x), abs(distance.y));

		// If there is no overlap, return
		if (minDistance.x <= 0 || minDistance.y <= 0) {
			return;
		}

		// Determine which direction to move the kinematic body
		if (minDistance.x < minDistance.y) {
			if (distance.x < 0) {
				position.x -= minDistance.x;
			}
			else {
				position.x += minDistance.x;
			}
		}
		else {
			if (distance.y < 0) {
				position.y -= minDistance.y;
			}
			else {
				position.y += minDistance.y;
			}
		}

	}

	void move() {


		position.x += velocity.x;
		position.y += velocity.y;

	}
};