#pragma once

#include "raylib.h"
#include "Object.h"

class VisualInstance : public Object {
protected:
	Vector2 size;
	Color color;
public:
	VisualInstance() {
		position = { 0, 0 };
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

	void draw() {
		DrawRectangle(position.x, position.y, size.x, size.y, color);
	}
};