#pragma once

#include "GLFW_Framework.h"
#include "MyMath.h"
#include "GameData.h"
#include "MyEnums.h"

struct BoxCollider
{
	BoxCollider();
	BoxCollider(Vector2D location, Vector2D dimensions);
	void Set(Vector2D location, Vector2D dimensions);

	int X;
	int Y;
	int WIDTH;
	int HEIGHT;

	static bool AreColliding(const BoxCollider& b1, const BoxCollider& b2);
};

class Actor
{
public:
	Actor();
	Actor(DrawLayer drawLayer);
	Actor(Vector2D location, DrawLayer drawLayer);
	Actor(Vector2D location, int digit, DrawLayer drawLayer);
	Actor(Vector2D location, std::string platformType, DrawLayer drawLayer);

	bool Init();
	void SetSprite(DrawLayer drawLayer);
	Sprite* GetSprite();

	void SetSpriteLocation(Vector2D location);
	Vector2D GetSpriteLocation();
	Vector2D GetSpriteDimensions();

	void SetColliderLocation(Vector2D location);
	Vector2D GetColliderLocation();
	Vector2D GetColliderDimensions();
	void SetColliderDimensions(int width, int height);

	BoxCollider GetBoxCollider();
	BoxCollider collider;

	void Destroy();
	static int counter;

	Vector2D projectileDestination;
	Vector2D cashedPlayerLocation;

	bool passedByPlayer;
	int digit;
	std::string platformType;
	bool toBeDestroyed;
	float timeAfterSteppedOn;
	float timeAfterShieldUsed;
private:

	Sprite* actorSprite;
	Vector2D sprite_location;
	Vector2D sprite_dimensions;

	Vector2D collider_dimensions;
	Vector2D collider_location;
};



