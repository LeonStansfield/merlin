#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "MerlinResources.h"
#include "MerlinResources.h"
#include "MerlinClass.h"

// Game Object
GameObject::GameObject()
{
	position = { 0, 0 };
	layer = 0;
}

GameObject::GameObject(Vector2 position)
{
	this->position = position;
}

Vector2 GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(Vector2 position)
{
	this->position = position;
}

void GameObject::ready(std::vector<GameObject*>& gameObjects)
{
}

void GameObject::update(vector<GameObject*> gameObjects)
{
}

void GameObject::end()
{
	delete this;
}

// Visual Instance

VisualInstance::VisualInstance()
{
	position = { 0.0, 0.0 };
	size = { 1, 1 };
	hasTexture = false;
	color = WHITE;
	texturePath = "";
}

VisualInstance::VisualInstance(Vector2 position, Vector2 size, bool hasTexture, Color color, string texturePath)
{
	this->position = position;
	this->size = size;
	this->hasTexture = hasTexture;
	this->color = color;
	this->texturePath = texturePath;
}

Vector2 VisualInstance::getSize()
{
	return size;
}

void VisualInstance::setSize(Vector2 size)
{
	this->size = size;
}

Color VisualInstance::getColor()
{
	return color;
}

void VisualInstance::setColor(Color color)
{
	this->color = color;
}

void VisualInstance::setVisible(bool visibility)
{
	this->visible = visibility;
}

bool VisualInstance::getVisible()
{
	return visible;
}

// overide ready() to load textures
void VisualInstance::ready(std::vector<GameObject*>& gameObjects)
{
	if (hasTexture)
	{
		texture = LoadTexture(texturePath.c_str());
	}
}

void VisualInstance::draw(Vector2 camera_offset)
{
	if (!visible)
		return;
	int drawPositionX = position.x - camera_offset.x;
	int drawPositionY = position.y - camera_offset.y;
	if (hasTexture)
	{
		DrawTexture(texture, drawPositionX, drawPositionY, color);
	}
	else
	{
		DrawRectangle(drawPositionX, drawPositionY, size.x, size.y, color);
	}
}

void VisualInstance::end()
{
	if (hasTexture)
	{
		UnloadTexture(texture);
	}
	delete this;
}

// Collision

Collision::Collision()
{
	position = { 0.0, 0.0 };
	size = { 1, 1 };
	layer = 1;
	hasTexture = false;
	color = MAROON;
	texturePath = "";
}

Collision::Collision(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath)
{
	this->position = position;
	this->size = size;
	this->layer = layer;
	this->hasTexture = hasTexture;
	this->color = color;
	this->texturePath = texturePath;
}

bool Collision::checkCollision(Collision other)
{
	return CheckCollisionRecs({ position.x, position.y, size.x, size.y }, { other.position.x, other.position.y, other.size.x, other.size.y });
}

void Collision::drawCollisionBox()
{
	DrawRectangleLines(position.x, position.y, size.x, size.y, color);
}

void Collision::setLayer(int layer)
{
	this->layer = layer;
}

int Collision::getLayer()
{
	return layer;
}

bool Collision::pget(Vector2 pos, int layer, const std::vector<GameObject*>& gameObjects)
{
	for (GameObject* gameObject : gameObjects)
	{
		Collision* collision = dynamic_cast<Collision*>(gameObject);
		if (collision != nullptr && collision->getLayer() == layer)
		{
			if (CheckCollisionPointRec(pos, { collision->position.x, collision->position.y, collision->size.x, collision->size.y }))
			{
				return true;
			}
		}
	}
	return false; // no collision found with the specified layer
}

// KinematicBody

KinematicBody::KinematicBody()
{
	position = { 0, 0 };
	size = { 1, 1 };
	layer = 1;
	hasTexture = false;
	color = WHITE;
	texturePath = "";
	velocity = { 0, 0 };
}

KinematicBody::KinematicBody(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath, Vector2 velocity)
{
	this->position = position;
	this->size = size;
	this->layer = layer;
	this->hasTexture = hasTexture;
	this->color = color;
	this->texturePath = texturePath;
	this->velocity = velocity;
}

Vector2 KinematicBody::getVelocity()
{
	return velocity;
}

void KinematicBody::setVelocity(Vector2 velocity)
{
	this->velocity = velocity;
}

// resolve collision
void KinematicBody::resolveCollision(Collision& other)
{
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
	if (overlapX > overlapY)
	{
		if (dy > 0)
		{
			moveUp = true;
		}
		else
		{
			moveDown = true;
		}
	}
	else
	{
		if (dx > 0)
		{
			moveLeft = true;
		}
		else
		{
			moveRight = true;
		}
	}

	// Move the kinematic body out of the collision box along the direction of the collision
	if (moveUp)
	{
		position.y += overlapY;
	}
	else if (moveDown)
	{
		position.y -= overlapY;
	}
	else if (moveLeft)
	{
		position.x += overlapX;
	}
	else if (moveRight)
	{
		position.x -= overlapX;
	}
}

void KinematicBody::processCollisions(vector<GameObject*> gameObjects)
{
	// check if colliding with all other objects in the game, and if so, resolve the collision
	for (GameObject* gameObject : gameObjects)
	{
		// Ignore self
		if (gameObject == this)
		{
			continue;
		}

		// Check if gameObject is collidable and colliding with player
		Collision* collision = dynamic_cast<Collision*>(gameObject);
		if (collision != nullptr && collision->checkCollision(*this))
		{
			// if this object is on the same layer as the other object, resolve the collision
			if ((collision->layer) == this->layer)
			{
				resolveCollision(*collision);
				continue;
			}
			else
			{
				continue;
			}
		}
		else
		{
		}
	}
}

void KinematicBody::move()
{

	position.x += velocity.x;
	position.y += velocity.y;
}

void KinematicBody::update(vector<GameObject*> gameObjects)
{
	// check if colliding with all other objects in the game, and if so, resolve the collision
	processCollisions(gameObjects);
}

// Tile

Tile::Tile(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath)
{
	this->position = position;
	this->size = size;
	this->layer = layer;
	this->hasTexture = hasTexture;
	this->color = color;
	this->texturePath = texturePath;
}


// Tilemap

Tilemap::Tilemap(string tileMapPath, int tileSize)
{
	this->tileMapPath = tileMapPath;
	this->tileSize = tileSize;
}

void Tilemap::spawnTiles(std::vector<GameObject*>& gameObjects)
{
	std::ifstream infile(tileMapPath);
	if (!infile)
	{
		std::cerr << "Failed to open file: " << tileMapPath << std::endl;
		return;
	}

	// Read in tilemap dimensions
	infile >> width >> height;

	// Create tile objects for non-empty tiles
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tileType;
			infile >> tileType;
			if (tileType == 1)
			{
				Tile* tile = new Tile({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { static_cast<float>(tileSize), static_cast<float>(tileSize) }, 1, false, green, "");
				gameObjects.push_back(tile);
			}
			else if (tileType == 2)
			{
				Tile* tile = new Tile({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { static_cast<float>(tileSize), static_cast<float>(tileSize) }, 2, false, orange, "");
				gameObjects.push_back(tile);
			}
		}
	}
}

void Tilemap::ready(std::vector<GameObject*>& gameObjects)
{
	spawnTiles(gameObjects);
}

// Scene manager
/*
void SceneManager::loadScene(std::vector<GameObject*>& gameObjects, string filename)
{
	// load file
	fstream file;
	file.open(filename, ios::in);
	if (!file.is_open())
	{
		cout << "Failed to open file: " << filename << endl;
		return;
	}
	else
	{
		cout << "File opened successfully: " << filename << endl;
	}

	// read file
	string line;
	int mode = 0; // 0 = none, 1 = looking for object type, 2 = looking for object name, 3 = looking for object data

	string objectType;
	string objectName;
	vector<string> objectData;

	int objectDataCount = 0;
	int typeDataCount = 0;

	while (getline(file, line))
	{
		if (mode == 0)
		{
			if (line == "object")
			{
				mode = 1;
				continue;
			}
		}
		if (mode == 1)
		{
			if (line == "End")
			{
				printf("End found, halting object ingress\n");
				mode = 0;
				continue;
			}
			else if (line == "Tilemap")
			{
				objectType = "Tilemap";
				typeDataCount = 2;
				mode = 2;
				continue;
			}
		}
		if (mode == 2)
		{
			if (line == "end")
			{
				printf("end found. Object was not complete. No object was created.\n");
				mode = 0;
				continue;
			}
			else
			{
				objectName = line;
				mode = 3;
				continue;
			}
		}
		if (mode == 3)
		{
			if (line == "end")
			{
				printf("end found. Object was not complete. No object was created.\n");
				mode = 0;
				continue;
			}
			else if (objectDataCount < typeDataCount)
			{
				objectData.push_back(line);
				objectDataCount++;
				if (objectDataCount == typeDataCount)
				{
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

	// close file
	file.close();
}

void SceneManager::changeScene(std::vector<GameObject*>& gameObjects, string filename)
{
	// remove all objects from gameObjects vector
	for (std::vector<GameObject*>::size_type i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	// remove the deleted pointers from the vector
	gameObjects.clear();

	// Create a new player object - current circular dependency issues - this should be done in the .msd file anyway
	//Player *player = new Player({64, 64}, {6, 8}, 1, true, white, "gameData/Textures/player.png", {0, 0});
	//gameObjects.push_back(player);

	// load new scene
	loadScene(gameObjects, filename);

	// proccess game objects ready function
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->ready(gameObjects);
	}
}

void createObject(std::vector<GameObject*>& gameObjects, string objectType, string objectName, std::vector<string>& objectData)
{
	cout << "Creating object of type: " << objectType << " with name: " << objectName << endl;
	for (std::vector<string>::size_type i = 0; i < objectData.size(); i++)
	{
		cout << "Data " << i << ": " << objectData[i] << endl;
	}

	if (objectType == "Tilemap")
	{
		if (objectData.size() < 2)
		{
			cout << "Error: Tilemap data vector should have at least two elements" << endl;
			return;
		}
		Tilemap* tilemap = new Tilemap(objectData[0], stoi(objectData[1]));
		gameObjects.push_back(tilemap);
		printf("Tilemap created\n");
	}
}
*/
