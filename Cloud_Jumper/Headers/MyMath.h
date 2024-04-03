#pragma once
#include "GLFW_Framework.h"
#include <iostream>

struct Vector2D;

class MyMath
{
public:
	MyMath();

	static int clamp(const int& value, const int& minValue, const int& maxValue);
	static float clamp(const float& value, const float& minValue, const float& maxValue);

	static Vector2D lerp(Vector2D& start, Vector2D& end, float t);
	static Vector2D quartic_erp(Vector2D& start, Vector2D& end, float t);
	static float easeInOut(float t);
	static int randInRange(int lowerBound, int upperBound);
};


struct Vector2D
{
	Vector2D();
	Vector2D(float x, float y);
	Vector2D operator+(const Vector2D& other);
	Vector2D operator-(const Vector2D& other);
	Vector2D operator*(float scalar);
	Vector2D operator/(float scalar);
	float dot(const Vector2D& other);
	void Normalize();
	static Vector2D ZeroVector();

	float X;
	float Y;

	//returns float X
	float getX_f();
	//returns float Y
	float getY_f();
};

