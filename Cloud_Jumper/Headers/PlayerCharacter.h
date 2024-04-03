#pragma once
#include "GLFW_Framework.h"
#include "MyMath.h"
#include "Drawer.h"
#include "Actor.h"
#include <unordered_map>


class PlayerCharacter
{
public:
	PlayerCharacter();

	bool Init();
	void SetSprite();
	Sprite* GetSprite();
	void SetSpriteLocation(Vector2D location);
	Vector2D GetSpriteLocation();
	Vector2D GetSpriteDimensions();

	void SetColliderLocation(Vector2D location);
	Vector2D GetColliderLocation();
	Vector2D GetColliderDimensions();
	void SetColliderDimensions(int X, int Y);

	BoxCollider collider_jump;
	BoxCollider collider_enemy;

	//movement
	void MoveOnInput(float input, float DeltaTime);
	void ChangeSpeedOnRightSide(int& speed, int& acc);
	void HandleOutOfBounds(Vector2D& newLocation);
	Vector2D movementVector;
	float rightInput;
	Vector2D rightVector;
	Vector2D leftVector;
	bool isMovingRight;
	bool isMovingLeft;
	float maxSpeed_left;
	float acceleration_left;
	int maxSpeed_right;
	int acceleration_right;
	Vector2D currentVelocity;

	//gravity
	void ApplyGravity(float DeltaTime);
	float gravityTerminalVelocity;
	float gravityAcceleration;
	Vector2D gravityDirection;
	Vector2D currentVerticalVelocity;

	//jumping
	bool isJumping;
	void Jump(float DeltaTime);
	float jumpTerminalVelocity;
	float jumpDecceleration;
	Vector2D jumpDirection;

	//collision
	void JumpOnCollision(std::unordered_map<int, Actor*> platforms, float DeltaTime);

	//camera movement
	void MoveCamera(std::unordered_map<int, Actor*> platforms, float DeltaTime);
	void MoveProjectiles(std::unordered_map<int, Actor*>& projectiles, float DeltaTime);

	//shield
	bool isImmune;

	int coinsCollected;

private:
	Sprite* playerSprite;
	Vector2D sprite_location;
	Vector2D sprite_dimensions;
	Vector2D collider_jump_location;
	Vector2D collider_jump_dimensions;
	Vector2D collider_enemy_location;
	Vector2D collider_enemy_dimensions;

	int screen_width;
	int screen_height;
};

