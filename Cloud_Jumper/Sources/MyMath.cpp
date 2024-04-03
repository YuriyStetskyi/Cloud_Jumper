#include "MyMath.h"

MyMath::MyMath()
{

}

int MyMath::clamp(const int& value, const int& minValue, const int& maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	else if (value > maxValue)
	{
		return maxValue;
	}
	else
	{
		return value;
	}
}

float MyMath::clamp(const float& value, const float& minValue, const float& maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	else if (value > maxValue)
	{
		return maxValue;
	}
	else
	{
		return value;
	}
}

Vector2D MyMath::lerp(Vector2D& start, Vector2D& end, float t)
{
	//t = Math::clamp(t, 0.0, 1.0);
	//return start + (end - start) * t;
	Vector2D temp(start.X + (end.X - start.X) * t, start.Y + (end.Y - start.Y) * t);
	return temp;
}

Vector2D MyMath::quartic_erp(Vector2D& start, Vector2D& end, float t)
{
	return Vector2D(start.X + (end.X - start.X) * std::pow(t, 4), start.Y + (end.Y - start.Y) * std::pow(t, 4));
}


float MyMath::easeInOut(float t)
{
	//return 0.5f * (1.0f - std::cos(3.14 * t));

	return (t * t * t * t) / 10;
}

int MyMath::randInRange(int lowerBound, int upperBound)
{
	return std::rand() % (upperBound - lowerBound + 1) + lowerBound;
}

Vector2D::Vector2D()
{

}

Vector2D::Vector2D(float x, float y)
{
	this->X = x;
	this->Y = y;
}

Vector2D Vector2D::operator+(const Vector2D& other)
{
	return Vector2D(this->X + other.X, this->Y + other.Y);
}

Vector2D Vector2D::operator-(const Vector2D& other)
{
	return Vector2D(this->X - other.X, this->Y - other.Y);
}

Vector2D Vector2D::operator*(float scalar)
{
	return Vector2D(this->X * scalar, this->Y * scalar);
}

Vector2D Vector2D::operator/(float scalar)
{
	return Vector2D(this->X / scalar, this->Y / scalar);
}

float Vector2D::dot(const Vector2D& other)
{
	return this->X * other.X + this->Y * other.Y;
}

void Vector2D::Normalize()
{
	float magnitude = std::sqrt(X * X + Y * Y);
	if (magnitude != 0.0)
	{
		X /= magnitude;
		Y /= magnitude;
	}
}

Vector2D Vector2D::ZeroVector()
{
	return Vector2D(0, 0);
}

float Vector2D::getX_f()
{
	return (float)X;
}

float Vector2D::getY_f()
{
	return (float)Y;
}

