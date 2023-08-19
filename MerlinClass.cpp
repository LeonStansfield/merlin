#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "raylib.h"
#include "GlobalVariables.h"
#include "MerlinClass.h"
#include "player.h"

// Game Object
GameObject::GameObject()
{
	position = { 0, 0 };
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
	renderLayer = 0;
	hasTexture = false;
	color = WHITE;
	texturePath = "";
}

VisualInstance::VisualInstance(Vector2 position, Vector2 size, bool hasTexture, Color color, string texturePath)
{
	this->position = position;
	this->size = size;
	this->renderLayer = 0;
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

int VisualInstance::getRenderLayer()
{
	return renderLayer;
}

void VisualInstance::setRenderLayer(int renderLayer)
{
	this->renderLayer = renderLayer;
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


// Animated Sprite

AnimatedSprite::AnimatedSprite() 
{
	this->visible = true;
	this->sprite = LoadTexture("");
	this->frameWidth = 8;
	this->frameHeight = 8;
	this->numFrames = 1;
	this->frameRate = 1;
	this->currentFrame = 0;
	this->frameTimer = 0.0f;
}

AnimatedSprite::AnimatedSprite(const std::string& spritePath, int frameWidth, int frameHeight, int numFrames, float frameRate)
{
	this->visible = true;
	this -> sprite = LoadTexture(spritePath.c_str());
	this -> frameWidth = frameWidth;
	this -> frameHeight = frameHeight;
	this -> numFrames = numFrames;
	this -> frameRate = frameRate;
	this -> currentFrame = 0;
	this -> frameTimer = 0.0f;
}

void AnimatedSprite::setVisible(bool visible) 
{
	this->visible = visible;
}

bool AnimatedSprite::getVisible()
{
	return this->visible;
}

void AnimatedSprite::setRenderLayer(int layer)
{
	this->renderLayer = layer;
}

int AnimatedSprite::getRenderLayer()
{
	return this->renderLayer;
}

void AnimatedSprite::setFrameWidth(int frameWidth)
{
	this->frameWidth = frameWidth;
}

int AnimatedSprite::getFrameWidth()
{
	return frameWidth;
}

void AnimatedSprite::setFrameHeight(int frameHeight)
{
	this->frameHeight = frameHeight;
}

int AnimatedSprite::getFrameHeight()
{
	return frameHeight;
}

void AnimatedSprite::update(vector<GameObject*> gameObjects)
{
	frameTimer += 1;
	if (frameTimer >= 1.0f / frameRate) 
	{
		frameTimer = 0.0f;
		currentFrame = (currentFrame + 1) % numFrames;
	}
}

void AnimatedSprite::draw(Vector2 camera_offset)
{
	if (!visible)
		return;
	float drawPositionX = position.x - camera_offset.x;
	float drawPositionY = position.y - camera_offset.y;

	Rectangle sourceRect = { static_cast<float>(currentFrame * frameWidth), 0, static_cast<float>(frameWidth), static_cast<float>(frameHeight) };
	DrawTextureRec(sprite, sourceRect, Vector2{ drawPositionX, drawPositionY }, WHITE);
}

void AnimatedSprite::end() 
{
	UnloadTexture(sprite);
	delete this;
}


// Collision

Collision::Collision()
{
	position = { 0.0, 0.0 };
	size = { 1, 1 };
	renderLayer = 0;
	collisionLayer = 1;
	hasTexture = false;
	color = MAROON;
	texturePath = "";
}

Collision::Collision(Vector2 position, Vector2 size, int collisionLayer, bool hasTexture, Color color, string texturePath)
{
	this->position = position;
	this->size = size;
	this->renderLayer = 0;
	this->collisionLayer = collisionLayer;
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
	this->collisionLayer = layer;
}

int Collision::getLayer()
{
	return collisionLayer;
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
	renderLayer = 0;
	collisionLayer = 1;
	hasTexture = false;
	color = WHITE;
	texturePath = "";
	velocity = { 0, 0 };
}

KinematicBody::KinematicBody(Vector2 position, Vector2 size, int collisionLayer, bool hasTexture, Color color, string texturePath, Vector2 velocity)
{
	this->position = position;
	this->size = size;
	this->renderLayer = 0;
	this->collisionLayer = collisionLayer;
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
		velocity.y = 0;
	}
	else if (moveDown)
	{
		position.y -= overlapY;
		velocity.y = 0;
	}
	else if (moveLeft)
	{
		position.x += overlapX;
		velocity.x = 0;
	}
	else if (moveRight)
	{
		position.x -= overlapX;
		velocity.x = 0;
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
			if ((collision->collisionLayer) == this->collisionLayer)
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

Tile::Tile(Vector2 position, Vector2 size, int collisionLayer, bool hasTexture, Color color, string texturePath)
{
	this->position = position;
	this->size = size;
	this->renderLayer = 0;
	this->collisionLayer = collisionLayer;
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

			switch (tileType)
			{
			case 0:
				// Empty tile, do nothing
				break;
			case 1:
			{
				Tile* tile = new Tile({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { static_cast<float>(tileSize), static_cast<float>(tileSize) }, 1, false, GREEN, "");
				gameObjects.push_back(tile);
				break;
			}
			case 2:
			{
				Tile* tile = new Tile({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { static_cast<float>(tileSize), static_cast<float>(tileSize) }, 2, false, ORANGE, "");
				gameObjects.push_back(tile);
				break;
			}
			case 3:
			{
				Player* player = new Player({ static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize }, { 6 , 8 }, 1, "gameData/Textures/player.png");
				player->setRenderLayer(1);
				gameObjects.push_back(player);
				GlobalVariables::GetInstance().playerReference = gameObjects.size() - 1;
				break;
			}
			case 4:
			{
				AnimatedSprite* animatedSprite = new AnimatedSprite("gameData/Textures/anim_test.png", 8, 8, 6, 10.0f);
				animatedSprite->setRenderLayer(1);
				gameObjects.push_back(animatedSprite);
				cout << "animated sprite created" << endl;
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
}


void Tilemap::ready(std::vector<GameObject*>& gameObjects)
{
	spawnTiles(gameObjects);
}
