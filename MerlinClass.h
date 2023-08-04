#pragma once

#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "raylib.h"


class GameObject
{
public:
    virtual ~GameObject() {} // virtual destructor
    Vector2 position;
    int layer;

    GameObject();

    GameObject(Vector2 position);

    Vector2 getPosition();

    void setPosition(Vector2 position);

    virtual void ready(std::vector<GameObject*>& gameObjects);

    virtual void update(vector<GameObject*> gameObjects);

    virtual void end();
};

class VisualInstance : public GameObject
{
public:
    bool visible = true;
    Vector2 size;
    bool hasTexture;
    Color color;
    string texturePath;
    Texture2D texture;

    VisualInstance();

    VisualInstance(Vector2 position, Vector2 size, bool hasTexture, Color color, string texturePath);

    Vector2 getSize();

    void setSize(Vector2 size);

    Color getColor();

    void setColor(Color color);

    void setVisible(bool visibility);

    bool getVisible();

    // overide ready() to load textures
    void ready(std::vector<GameObject*>& gameObjects) override;

    void draw(Vector2 camera_offset);

    void end() override;
};


class Collision : public VisualInstance
{

public:
    int layer = 1;

    Collision();

    Collision(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath);

    bool checkCollision(Collision other);

    void drawCollisionBox();

    void setLayer(int layer);

    int getLayer();

    bool pget(Vector2 pos, int layer, const std::vector<GameObject*>& gameObjects);
};


class KinematicBody : public Collision
{
public:
    Vector2 velocity;

    KinematicBody();

    KinematicBody(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath, Vector2 velocity);

    Vector2 getVelocity();

    void setVelocity(Vector2 velocity);

    // resolve collision
    void resolveCollision(Collision& other);

    void processCollisions(vector<GameObject*> gameObjects);

    void move();

    void update(vector<GameObject*> gameObjects) override;
};


class Tile : public Collision
{
    // tile class, a tile is spawned by the tilemap class and is a collision object.
public:
    // Constructor with arguments
    Tile(Vector2 position, Vector2 size, int layer, bool hasTexture, Color color, string texturePath);
};

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

/*
class SceneManager
{
public:
    void loadScene(std::vector<GameObject*>& gameObjects, string filename);

    void changeScene(std::vector<GameObject*>& gameObjects, string filename);

    void createObject(std::vector<GameObject*>& gameObjects, string objectType, string objectName, std::vector<string>& objectData);
};
*/