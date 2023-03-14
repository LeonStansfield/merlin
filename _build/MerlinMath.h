#pragma once

#include <math.h>
#include <vector>
#include <iostream>
#include "raylib.h"

Vector2 Vector2Add(Vector2 vectorA, Vector2 VectorB) {
	return { vectorA.x + VectorB.x, vectorA.y + VectorB.y };
}

Vector2 Vector2Subtract(Vector2 vectorA, Vector2 VectorB) {
	return { vectorA.x - VectorB.x, vectorA.y - VectorB.y };
}

Vector2 Vector2Multiply(Vector2 vectorA, float amount) {
	return { vectorA.x * amount, vectorA.y * amount };
}