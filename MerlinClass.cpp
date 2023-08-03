#include <vector>
#include <fstream>
#include <string>
using namespace std;

#include "raylib.h"
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