#pragma once

#include <vector>
#include <fstream>

#include "raylib.h"
#include "MerlinMath.h"

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

    virtual void ready(std::vector<GameObject*>& gameObjects) {
    }

    virtual void update(vector<GameObject*> gameObjects) {
    }
};

class VisualInstance : public GameObject {
public:
    bool visible = true;
    Vector2 size;
    Color color;

    VisualInstance() {
        position = { 0.0, 0.0 };
        size = { 1, 1 };
        color = WHITE;
    }

    VisualInstance(Vector2 position, Vector2 size, Color color) {
        this->position = position;
        this->size = size;
        this->color = color;
    }

    Vector2 getSize() {
        return size;
    }

    void setSize(Vector2 size) {
        this->size = size;
    }

    Color getColor() {
        return color;
    }

    void setColor(Color color) {
        this->color = color;
    }

    void setVisible(bool visibility) {
        this->visible = visibility;
    }

    bool getVisible() {
        return visible;
    }

    void draw() {
        DrawRectangle(position.x, position.y, size.x, size.y, color);
    }
};

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

    void processCollisions(vector<GameObject*> gameObjects){
        //check if colliding with all other objects in the game, and if so, resolve the collision
		for (GameObject* gameObject : gameObjects)
		{
			// Ignore self
			if (gameObject == this) {
				continue;
			}

			// Check if gameObject is collidable and colliding with player
			Collision* collision = dynamic_cast<Collision*>(gameObject);
			if (collision != nullptr && collision->checkCollision(*this))
			{
				setColor(RED);
				resolveCollision(*collision);
				continue;
			}
			else {
				setColor(BLUE);
			}
		}
    }

	void move() {


		position.x += velocity.x;
		position.y += velocity.y;

	}

    void update(vector<GameObject*> gameObjects) override {
        //check if colliding with all other objects in the game, and if so, resolve the collision
        processCollisions(gameObjects);
    }
};

class Tile : public Collision
{
	// tile class, a tile is spawned by the tilemap class and is a collision object.
public:
	// Constructor with arguments
	Tile(Vector2 position, Vector2 size, Color color)
	{
		this->position = position;
		this->size = size;
		this->color = color;
	}
};

class Tilemap : public GameObject
{
private:
	string tilemapPath;
	int tileSize;
	int width;
	int height;

public:

	Tilemap(string tilemapPath, int tileSize)
	{
		this->tilemapPath = tilemapPath;
		this->tileSize = tileSize;
	}

	void spawnTiles(std::vector<GameObject*>& gameObjects)
	{
        std::ifstream infile(tilemapPath);
        if (!infile) {
            std::cerr << "Failed to open file: " << tilemapPath << std::endl;
            return;
        }

        // Read in tilemap dimensions
        infile >> width >> height;

		// Create tile objects for non-empty tiles
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int tileType;
                infile >> tileType;
                if (tileType == 1) {
					Tile *tile = new Tile({static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize}, {static_cast<float>(tileSize), static_cast<float>(tileSize)}, RED);
					gameObjects.push_back(tile);
				}
                else if (tileType == 2) {
                    Tile *tile = new Tile({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { static_cast<float>(tileSize), static_cast<float>(tileSize) }, GREEN);
                    gameObjects.push_back(tile);
                }
            }
        }


	}

	void ready(std::vector<GameObject*>& gameObjects) override
	{
		spawnTiles(gameObjects);
	}
};
