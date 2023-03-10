#pragma once

#include "Object.h"

class VisualInstance: Object {
private:
	Vector2 size;
	Color color;

public:
	VisualInstance(Vector2 position, Vector2 size, Color color) {
		this->position = position;
		this->size = size;
		this->color = color;
	}

	Vector2 getSize() {
		return size;
	}

	Color getColor() {
		return color;
	}

	void setColor(Color color) {
		this->color = color;
	}

	void setSize(Vector2 size) {
		this->size = size;
	}

	void draw() {
		DrawRectangle(this->position.x, this->position.y, this->size.x, this->size.y, this->color);
	}
};