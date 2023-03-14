#pragma once

#include <vector>

#include "raylib.h"
#include "Collision.h"
#include "MerlinMath.h"


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
        // Find the distance between the centers of the two colliding objects along the x and y axes
        float dx = (position.x + size.x / 2) - (other.position.x + other.size.x / 2);
        float dy = (position.y + size.y / 2) - (other.position.y + other.size.y / 2);

        // Find the sum of the half-widths and half-heights of the two colliding objects along the x and y axes
        float combinedHalfWidth = (size.x + other.size.x) / 2;
        float combinedHalfHeight = (size.y + other.size.y) / 2;

        // Calculate the overlap along the x and y axes
        float overlapX = combinedHalfWidth - abs(dx);
        float overlapY = combinedHalfHeight - abs(dy);

        // Determine the direction of the collision
        bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;
        if (overlapX > overlapY) {
            if (dy > 0) {
                moveUp = true;
            }
            else {
                moveDown = true;
            }
        }
        else {
            if (dx > 0) {
                moveLeft = true;
            }
            else {
                moveRight = true;
            }
        }

        // Move the kinematic body out of the collision box along the direction of the collision
        if (moveUp) {
            position.y += overlapY;
        }
        else if (moveDown) {
            position.y -= overlapY;
        }
        else if (moveLeft) {
            position.x += overlapX;
        }
        else if (moveRight) {
            position.x -= overlapX;
        }
    }

	void move() {


		position.x += velocity.x;
		position.y += velocity.y;

	}
};