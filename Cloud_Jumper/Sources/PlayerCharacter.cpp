#include "PlayerCharacter.h"




PlayerCharacter::PlayerCharacter()
	:playerSprite(nullptr),
	sprite_location(0, 0),
	sprite_dimensions(0, 0),
	rightInput(0.0f),
	rightVector(1, 0),
	leftVector(-1, 0),
	movementVector(0, 0),
	maxSpeed_left(400.0f),
	acceleration_left(15.0f),
	currentVelocity(0, 0),
	isMovingRight(false),
	isMovingLeft(false),
	gravityTerminalVelocity(600.0f),
	gravityAcceleration(5.0f),
	gravityDirection(0, 1),
	currentVerticalVelocity(0, 0),
	jumpTerminalVelocity(1000),
	jumpDecceleration(5.0f),
	jumpDirection(0, -1),
	screen_width(0),
	screen_height(0),
	isImmune(false),
	coinsCollected(0)
{

	maxSpeed_right = maxSpeed_left * 1.5;
	acceleration_right = acceleration_left * 1.5;

	SetSprite();
	getScreenSize(screen_width, screen_height);

	SetColliderDimensions(GetSpriteDimensions().X, GetSpriteDimensions().Y);
	SetSpriteLocation(Vector2D(screen_width / 2, screen_height * 0.15));
	collider_jump.Set(collider_jump_location, collider_jump_dimensions);


}

bool PlayerCharacter::Init()
{


	return true;
}

void PlayerCharacter::SetSprite()
{
	playerSprite = GameData::spr_player.sprite;
	int X, Y;
	getSpriteSize(playerSprite, X, Y);
	sprite_dimensions.X = X;
	sprite_dimensions.Y = Y;
}

Sprite* PlayerCharacter::GetSprite()
{
	return this->playerSprite;
}

void PlayerCharacter::SetSpriteLocation(Vector2D location)
{
	this->sprite_location = location;
	SetColliderLocation(location);
}

Vector2D PlayerCharacter::GetSpriteLocation()
{
	return this->sprite_location;
}

Vector2D PlayerCharacter::GetSpriteDimensions()
{
	return sprite_dimensions;
}

void PlayerCharacter::SetColliderLocation(Vector2D location)
{
	Vector2D adjustedLocation(GetSpriteLocation().X + (GetSpriteDimensions().X * 0.4), GetSpriteLocation().Y + (GetSpriteDimensions().Y * 0.95));
	this->collider_jump_location = adjustedLocation;
	collider_jump.X = adjustedLocation.X;
	collider_jump.Y = adjustedLocation.Y;

	collider_enemy.X = GetSpriteLocation().X;
	collider_enemy.Y = GetSpriteLocation().Y;
}

Vector2D PlayerCharacter::GetColliderLocation()
{
	return collider_jump_location;
}

Vector2D PlayerCharacter::GetColliderDimensions()
{
	return collider_jump_dimensions;
}

void PlayerCharacter::SetColliderDimensions(int X, int Y)
{
	if (X && Y)
	{
		collider_jump_dimensions.X = X * 0.2;
		collider_jump_dimensions.Y = Y * 0.05;
		collider_jump.WIDTH = X * 0.2;
		collider_jump.HEIGHT = Y * 0.05;
		
		collider_enemy_dimensions.X = X;
		collider_enemy_dimensions.Y = Y;
		collider_enemy.WIDTH = X;
		collider_enemy.HEIGHT = Y;
	}
}



void PlayerCharacter::MoveOnInput(float input, float DeltaTime)
{
	int maxSpeed = maxSpeed_left;
	int acceleration = acceleration_left;

	//ChangeSpeedOnRightSide(maxSpeed, acceleration);

	if (input > 0)
	{
		movementVector = rightVector;
	}
	else if (input < 0)
	{
		movementVector = leftVector;
	}
	else
	{
		movementVector = Vector2D::ZeroVector();
	}

	Vector2D targetVelocity = movementVector * maxSpeed;
	currentVelocity = MyMath::lerp(currentVelocity, targetVelocity, acceleration * DeltaTime);
	Vector2D newLocation = GetSpriteLocation() + currentVelocity * DeltaTime;

	HandleOutOfBounds(newLocation);
	SetSpriteLocation(newLocation);

	if (!isJumping)
	{
		ApplyGravity(DeltaTime);
	}
	else
	{
		Jump(DeltaTime);
	}

	if (!isMovingRight && !isMovingLeft)
		rightInput = 0; //if player not pressing any buttons - stop	
}

void PlayerCharacter::ChangeSpeedOnRightSide(int& speed, int& acc)
{
	//player moves 50% faster on right side of screen
	if (GetSpriteLocation().X > screen_width / 2)
	{
		speed = maxSpeed_right;
		acc = acceleration_right;
	}
	else
	{
		speed = maxSpeed_left;
		acc = acceleration_left;
	}
}

void PlayerCharacter::HandleOutOfBounds(Vector2D& newLocation)
{
	if (newLocation.X > screen_width)
	{
		newLocation.X = 1;
	}
	else if (newLocation.X < 0)
	{
		newLocation.X = screen_width - 1;
	}
}

void PlayerCharacter::ApplyGravity(float DeltaTime)
{
	if (DeltaTime < 1)
	{
		Vector2D targetVelocity = gravityDirection * gravityTerminalVelocity;
		currentVerticalVelocity = MyMath::lerp(currentVerticalVelocity, targetVelocity, gravityAcceleration * DeltaTime);

		Vector2D newLocation = GetSpriteLocation() + currentVerticalVelocity * DeltaTime;


		SetSpriteLocation(newLocation);
	}

}

void PlayerCharacter::Jump(float DeltaTime)
{
	if (DeltaTime < 1)
	{
		Vector2D startingVelocity = jumpDirection * jumpTerminalVelocity;
		Vector2D stopVelocity(0, startingVelocity.Y * 0.1);

		Vector2D zero = Vector2D::ZeroVector();
		currentVerticalVelocity = MyMath::lerp(currentVerticalVelocity, zero, jumpDecceleration * DeltaTime);
		Vector2D newLocation = GetSpriteLocation() + currentVerticalVelocity * DeltaTime;

		if (currentVerticalVelocity.Y < stopVelocity.Y && DeltaTime < 1 && !(sprite_location.Y < (screen_height * 0.5)))
		{
			SetSpriteLocation(newLocation);
		}
		else if (!(currentVerticalVelocity.Y < stopVelocity.Y))
		{
			isJumping = false;
		}

	}

}

void PlayerCharacter::JumpOnCollision(std::unordered_map<int, Actor*> platforms, float DeltaTime)
{
	for (std::pair<int, Actor*> platform : platforms)
	{
		if (!isJumping && BoxCollider::AreColliding(collider_jump, platform.second->collider))
		{
			isJumping = true;
			Vector2D startingVelocity = jumpDirection * jumpTerminalVelocity;
			currentVerticalVelocity = startingVelocity;
			Jump(DeltaTime);

			if (platform.second->platformType == "timed")
				platform.second->toBeDestroyed = true;

			GLFW_Framework::SoundEngine->play2D(GameData::soundPaths.at(Sounds::SOUND_JUMP)->getSoundSource(), false);
		}
	}
}

void PlayerCharacter::MoveCamera(std::unordered_map<int, Actor*> platforms, float DeltaTime)
{
	//if player is above screen move platforms instead of player
	if (sprite_location.Y < (screen_height * 0.5) && DeltaTime < 1)
	{
		for (std::pair<int, Actor*> platform : platforms)
		{
			if (currentVerticalVelocity.Y < 0)
			{
				Vector2D newlocation = platform.second->GetSpriteLocation() - currentVerticalVelocity * DeltaTime;
				platform.second->SetSpriteLocation(newlocation);
			}
		}

		//track score
		if (currentVerticalVelocity.Y < 0)
			GameData::trackedScore -= currentVerticalVelocity.Y * DeltaTime;
	}
}

void PlayerCharacter::MoveProjectiles(std::unordered_map<int, Actor*>& projectiles, float DeltaTime)
{
	//if player is above screen move platforms instead of player

	std::unordered_map<int, Actor*> temp_projectiles = projectiles;
	for (std::pair<int, Actor*> projectile : projectiles)
	{
		Vector2D projectileDirection(projectile.second->cashedPlayerLocation.X - projectile.second->projectileDestination.X,
			projectile.second->cashedPlayerLocation.Y - projectile.second->projectileDestination.Y);
		projectileDirection.Normalize();
		Vector2D projectilevelocity(-projectileDirection.X * 600, -projectileDirection.Y * 600);

		Vector2D newLocation = projectile.second->GetSpriteLocation() + projectilevelocity * DeltaTime;

		if (newLocation.X > screen_width)
		{
			newLocation.X = 1;
		}
		else if (newLocation.X < 0)
		{
			newLocation.X = screen_width - 1;
		}


		projectile.second->SetSpriteLocation(newLocation);
		if (newLocation.Y > screen_height || newLocation.Y < 0)
		{
			temp_projectiles.erase(projectile.first);
		}
	}
	projectiles = temp_projectiles;


}







