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

    bool pget(Vector2 pos, int layer, const std::vector<GameObject*>& gameObjects){
        for (GameObject* gameObject : gameObjects) {
            Collision* collision = dynamic_cast<Collision*>(gameObject);
            if (collision != nullptr && collision->getLayer() == layer) {
                if (CheckCollisionPointRec(pos, { collision->position.x, collision->position.y, collision->size.x, collision->size.y })) {
                    return true;
                }
            }
        }
        return false; // no collision found with the specified layer
    }
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
private:
    string filename;

public:
    Scene(string filename){
        this->filename = filename;
    }

	void load(std::vector<GameObject*>& gameObjects)
    {
        //load file
        fstream file;
        file.open(filename, ios::in);
        if (!file.is_open())
        {
            cout << "Failed to open file: " << filename << endl;
            return;
        }
        else{
            cout << "File opened successfully: " << filename << endl;
        }

        //read file
        string line;
        int mode = 0; // 0 = none, 1 = looking for object type, 2 = looking for object name, 3 = looking for object data
        
        string objectType;
        string objectName;
        vector<string> objectData;

        int objectDataCount = 0;
        int typeDataCount = 0;

        while (getline(file, line))
        {
            if (mode == 0){
                if (line == "object"){
                    mode = 1;
                    continue;
                }
            }
            if (mode == 1){
                if (line == "End"){
                    printf("End found, halting object ingress\n");
                    mode = 0;
                    continue;
                }
                else if (line == "Tilemap"){
                    objectType = "Tilemap";
                    typeDataCount = 2;
                    mode = 2;
                    continue;
                }
                else if (line == "Player"){
                    objectType = "Player";
                    typeDataCount = 7;
                    mode = 2;
                    continue;
                }
            }
            if (mode == 2){
                if (line == "end"){
                    printf("end found. Object was not complete. No object was created.\n");
                    mode = 0;
                    continue;
                }
                else{
                    objectName = line;
                    mode = 3;
                    continue;
                }
            }
            if (mode == 3){
                if (line == "end"){
                    printf("end found. Object was not complete. No object was created.\n");
                    mode = 0;
                    continue;
                }
                else if (objectDataCount < typeDataCount){
                    objectData.push_back(line);
                    objectDataCount++;
                    if (objectDataCount == typeDataCount){
                        createObject(gameObjects, objectType, objectName, objectData);
                        objectData.clear();
                        objectDataCount = 0;
                        typeDataCount = 0;
                        mode = 0;
                        continue;
                    }
                }
            }
        }

        //close file
        file.close();
	}

    void createObject(std::vector<GameObject*>& gameObjects, string objectType, string objectName, std::vector<string> & objectData){
        cout << "Creating object of type: " << objectType << " with name: " << objectName << endl;
        for (std::vector<string>::size_type i = 0; i < objectData.size(); i++){
            cout << "Data " << i << ": " << objectData[i] << endl;
        }
        return;
    }

};