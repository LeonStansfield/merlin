#pragma once

#include <vector>
#include <fstream>

#include "raylib.h"
#include "MerlinResources.h"
#include "MerlinMath.h"

class GameObject {
public:
    virtual ~GameObject() {} // virtual destructor
    Vector2 position;
    int layer;

    GameObject() {
        position = { 0, 0 };
        layer = 0;
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

    virtual void end() {
        delete this;
    }
};

class VisualInstance : public GameObject {
public:
    bool visible = true;
    Vector2 size;
    bool hasTexture;
    Color color;
    string texturePath;
    Texture2D texture;

    VisualInstance() {
        position = { 0.0, 0.0 };
        size = { 1, 1 };
        hasTexture = false;
        color = WHITE;
        texturePath = "";
    }

    VisualInstance(Vector2 position, Vector2 size, bool hasTexture, Color color, string texturePath) {
        this->position = position;
        this->size = size;
        this->hasTexture = hasTexture;
        this->color = color;
        this->texturePath = texturePath;
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

    //overide ready() to load textures
    void ready(std::vector<GameObject*>& gameObjects) override {
        if (hasTexture) {
            texture = LoadTexture(texturePath.c_str());
        }
    }

    void draw(Vector2 camera_offset) {
        if (!visible) return;
        int drawPositionX = position.x - camera_offset.x;
        int drawPositionY = position.y - camera_offset.y;
        if (hasTexture){
            DrawTexture(texture, drawPositionX, drawPositionY, color);
        }
        else {
            DrawRectangle(drawPositionX , drawPositionY, size.x, size.y, color);
        }
    }

    void end() override {
        if (hasTexture) {
            UnloadTexture(texture);
        }
        delete this;
    }
};

class AnimatedSprite : public VisualInstance {
public:
    int frameCount;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    Texture2D frames[100];
    Rectangle sourceRec;

    AnimatedSprite() {
        position = { 0.0, 0.0 };
        size = { 1, 1 };
        hasTexture = true;
        color = WHITE;
        texturePath = "";
        frameCount = 1;
        currentFrame = 0;
        framesCounter = 0;
        framesSpeed = 8;
        sourceRec = { 0.0f, 0.0f, (float)size.x, (float)size.y };
    }

    AnimatedSprite(Vector2 position, Vector2 size, string texturePath, int frameCount, int framesSpeed) {
        this->position = position;
        this->size = size;
        this->hasTexture = true;
        this->color = WHITE;
        this->texturePath = texturePath;
        this->frameCount = frameCount;
        this->framesSpeed = framesSpeed;
        currentFrame = 0;
        framesCounter = 0;
        sourceRec = { 0.0f, 0.0f, (float)size.x, (float)size.y };
        texture = LoadTexture(texturePath.c_str());
    }

    int getFrameCount() {
        return frameCount;
    }

    void setFrameCount(int frameCount) {
        this->frameCount = frameCount;
    }

    int getCurrentFrame() {
        return currentFrame;
    }

    void setCurrentFrame(int currentFrame) {
        this->currentFrame = currentFrame;
    }

    int getFramesCounter() {
        return framesCounter;
    }

    void setFramesCounter(int framesCounter) {
        this->framesCounter = framesCounter;
    }

    int getFramesSpeed() {
        return framesSpeed;
    }

    void setFramesSpeed(int framesSpeed) {
        this->framesSpeed = framesSpeed;
    }

    Rectangle getSourceRec() {
        return sourceRec;
    }

    void setSourceRec(Rectangle sourceRec) {
        this->sourceRec = sourceRec;
    }

    void update(vector<GameObject*> gameObjects) override {
        framesCounter++;
        if (framesCounter >= (GetFPS() / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame > frameCount - 1) currentFrame = 0;
            float x = (float)currentFrame * (float)size.x;
            float y = 0.0f;
            float width = (float)size.x;
            float height = (float)size.y;
            setSourceRec({ x, y, width, height });
        }
    }

    void draw(Vector2 camera_offset) {
        if (!visible) return;
        float drawPositionX = position.x - camera_offset.x;
        float drawPositionY = position.y - camera_offset.y;
        DrawTextureRec(texture, sourceRec, {drawPositionX, drawPositionY}, color);
        printf("%f, %f, %f, %f \n", sourceRec.x, sourceRec.y, sourceRec.width, sourceRec.height);
    }
};

class Collision : public VisualInstance {

public:
    int layer = 1;

    Collision() {
        position = { 0.0, 0.0 };
        size = { 1, 1 };
        layer = 1;
        hasTexture = false;
        color = MAROON;
        texturePath = "";
    }

    Collision(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath) {
        this->position = position;
        this->size = size;
        this->layer = layer;
        this->hasTexture = hasTexture;
        this->color = color;
        this->texturePath = texturePath;
    }

    bool checkCollision(Collision other) {
        return CheckCollisionRecs({ position.x, position.y, size.x, size.y }, { other.position.x, other.position.y, other.size.x, other.size.y });
    }

    void drawCollisionBox() {
        DrawRectangleLines(position.x, position.y, size.x, size.y, color);
    }

    void setLayer(int layer) {
        this->layer = layer;
    }

    int getLayer() {
        return layer;
    }
};

class Rigidbody : public Collision {
    public:
        Vector2 velocity;
        Vector2 acceleration;
        float mass;
};

class KinematicBody : public Collision {
public:
	Vector2 velocity;

	KinematicBody() {
		position = { 0, 0 };
		size = { 1, 1 };
        layer = 1;
        hasTexture = false;
		color = WHITE;
        texturePath = "";
		velocity = { 0, 0 };
	}

	KinematicBody(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath, Vector2 velocity) {
		this->position = position;
		this->size = size;
        this->layer = layer;
        this->hasTexture = hasTexture;
		this->color = color;
        this->texturePath = texturePath;
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
                //if this object is on the same layer as the other object, resolve the collision
                if ((collision->layer) == this->layer) {
                    resolveCollision(*collision);
                    continue;
                }
                else {
                    continue;
                }
			}
			else {
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
	Tile(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath)
    {
        this->position = position;
        this->size = size;
        this->layer = layer;
        this->hasTexture = hasTexture;
        this->color = color;
        this->texturePath = texturePath;
    }
};

class Tilemap : public GameObject
{
private:
	string tileMapPath;
	int tileSize;
	int width;
	int height;

public:

	Tilemap(string tileMapPath, int tileSize)
	{
		this->tileMapPath = tileMapPath;
		this->tileSize = tileSize;
	}

	void spawnTiles(std::vector<GameObject*>& gameObjects)
	{
        std::ifstream infile(tileMapPath);
        if (!infile) {
            std::cerr << "Failed to open file: " << tileMapPath << std::endl;
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
					Tile *tile = new Tile({static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize}, {static_cast<float>(tileSize), static_cast<float>(tileSize)}, 1, false, green, "");
					gameObjects.push_back(tile);
				}
                else if (tileType == 2) {
                    Tile *tile = new Tile({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { static_cast<float>(tileSize), static_cast<float>(tileSize) }, 2, false, orange, "");
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

class Scene{
    
};