#pragma once

#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "raylib.h"

// Game Object
// GameObject is the base class for all game objects, holds a position
class GameObject
{
public:
    virtual ~GameObject() {} // virtual destructor
    Vector2 position;

    GameObject();

    GameObject(Vector2 position);

    Vector2 getPosition();

    void setPosition(Vector2 position);

    virtual void ready(std::vector<GameObject*>& gameObjects);

    virtual void update(vector<GameObject*> gameObjects);

    virtual void end();
};

// Visual Intance
// Draws a colored box or texture at a position, size, and render layer
class VisualInstance : public GameObject
{
public:
    bool visible = true;
    Vector2 size;
    int renderLayer;
    bool hasTexture;
    Color color;
    string texturePath;
    Texture2D texture;

    VisualInstance();

    VisualInstance(Vector2 position, Vector2 size, bool hasTexture, Color color, string texturePath);
    
    Vector2 getSize();

    void setSize(Vector2 size);

    int getRenderLayer();

    void setRenderLayer(int renderLayer);

    Color getColor();

    void setColor(Color color);

    void setVisible(bool visibility);

    bool getVisible();

    // Overide ready() to load textures
    void ready(std::vector<GameObject*>& gameObjects) override;

    void draw(Vector2 camera_offset);

    void end() override;
};

// Animated sprite
// Draws a texture2D at a position, size, and render layer, and animates it
class AnimatedSprite : public GameObject {
public:
    bool visible;
    Texture2D sprite;
    int renderLayer;
    int frameWidth;
    int frameHeight;
    int numFrames;
    float frameRate;
    int currentFrame;
    float frameTimer;

    AnimatedSprite();

    AnimatedSprite(const std::string& spritePath, int frameWidth, int frameHeight, int numFrames, float frameRate);

    bool getVisible();

    void setVisible(bool visible);

    int getRenderLayer();

    void setRenderLayer(int layer);

    int getFrameWidth();

    void setFrameWidth(int frameWidth);

    int getFrameHeight();

    void setFrameHeight(int frameHeight);

    void update(vector<GameObject*> gameObjects) override;

    void draw(Vector2 camera_offset);

    void end() override;
};

// Collision
// A collision box that kinematic bodies on the same layer will collide with
class Collision : public VisualInstance
{

public:
    int collisionLayer = 1;

    Collision();

    Collision(Vector2 position, Vector2 size, int collisionLayer, bool hasTexture, Color color, string texturePath);

    bool checkCollision(Collision other);

    void drawCollisionBox();

    void setLayer(int layer);

    int getLayer();

    bool pget(Vector2 pos, int layer, const std::vector<GameObject*>& gameObjects);
};

// Kinematic Body
// A collision box that can move at its velocity and resolve collision with other collision boxes
class KinematicBody : public Collision
{
public:
    Vector2 velocity;

    KinematicBody();

    KinematicBody(Vector2 position, Vector2 size, int collisionLayer, bool hasTexture, Color color, string texturePath, Vector2 velocity);

    Vector2 getVelocity();

    void setVelocity(Vector2 velocity);

    // resolve collision
    void resolveCollision(Collision& other);

    void processCollisions(vector<GameObject*> gameObjects);

    void move();

    void update(vector<GameObject*> gameObjects) override;
};

// Tile
// A collision box that is spawned by the tilemap class
class Tile : public Collision
{
    // tile class, a tile is spawned by the tilemap class and is a collision object.
public:
    // Constructor with arguments
    Tile(Vector2 position, Vector2 size, int collisionLayer, bool hasTexture, Color color, string texturePath);
};

// Foreward declarations for classes that can be spawned by the tilemap that are not declared in MerlinClass.h
class player;

// Tilemap
// Spawns tiles and other objects from a text file
class Tilemap : public GameObject
{
private:
    string tileMapPath;
    int tileSize;
    int width;
    int height;

public:
    Tilemap(string tileMapPath, int tileSize);

    void spawnTiles(std::vector<GameObject*>& gameObjects);

    void ready(std::vector<GameObject*>& gameObjects) override;
};
