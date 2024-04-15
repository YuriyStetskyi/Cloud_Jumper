#include "Actor.h"


BoxCollider::BoxCollider()
{
}

BoxCollider::BoxCollider(Vector2D location, Vector2D dimensions)
{
	X = location.X;
	Y = location.Y;
	WIDTH = dimensions.X;
	HEIGHT = dimensions.Y;
}

void BoxCollider::Set(Vector2D location, Vector2D dimensions)
{
	X = location.X;
	Y = location.Y;
	WIDTH = dimensions.X;
	HEIGHT = dimensions.Y;
}

bool BoxCollider::isHovered(Vector2D mouseLocation)
{
	if (mouseLocation.X >= X && mouseLocation.X <= X + WIDTH
		&& mouseLocation.Y >= Y && mouseLocation.Y <= Y + HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BoxCollider::AreColliding(const BoxCollider& b1, const BoxCollider& b2)
{
	return (b1.X < b2.X + b2.WIDTH &&
		b1.X + b1.WIDTH > b2.X &&
		b1.Y < b2.Y + b2.HEIGHT &&
		b1.Y + b1.HEIGHT > b2.Y);
}

Actor::Actor()
{

}

Actor::Actor(DrawLayer drawLayer)
	:actorSprite(nullptr),
	sprite_location(0, 0),
	sprite_dimensions(0, 0),
	collider_location(0, 0),
	collider_dimensions(0, 0),
	projectileDestination(0, 0),
	cashedPlayerLocation(0, 0),
	passedByPlayer(false),
	digit(0),
	spriteType(""),
	toBeDestroyed(false),
	timeAfterSteppedOn(0.0f),
	timeAfterShieldUsed(0.0f)
{
	SetSprite(drawLayer);
	SetColliderDimensions(GetSpriteDimensions().X, GetSpriteDimensions().Y);
	collider.Set(collider_location, collider_dimensions);

	counter++;
	//all the different actors(platform,player,coin,enemy) should be inherited from Actor
	//but unfortunately made bad decision to postpone refactoring that part till end and theres no time left
	//should have made inheritance from the beginning.
}

Actor::Actor(Vector2D location, DrawLayer drawLayer)
	:Actor::Actor(drawLayer)
{
	SetSpriteLocation(location);
}

Actor::Actor(Vector2D location, int digit, DrawLayer drawLayer)
	:Actor::Actor(location, drawLayer)
{
	this->digit = digit;
	SetSprite(drawLayer); //calling this twice is bad and a crutch - fix later
}

Actor::Actor(Vector2D location, std::string spriteType, DrawLayer drawLayer)
	:Actor::Actor(location, drawLayer)
{
	this->spriteType = spriteType;
	SetSprite(drawLayer);
	SetColliderDimensions(GetSpriteDimensions().X, GetSpriteDimensions().Y);
	collider.Set(collider_location, collider_dimensions);
}

int Actor::counter = 0;



bool Actor::Init()
{


	return true;
}


void Actor::SetSprite(DrawLayer drawLayer)
{
	switch (drawLayer)
	{
	case BACKGROUND:
	{
		actorSprite = GameData::spr_background.sprite;
		sprite_dimensions.X = GameData::spr_background.width;
		sprite_dimensions.Y = GameData::spr_background.height;
		break;
	}
	case MAINMENU:
	{
		if (spriteType == "button_Play_passive")
		{
			actorSprite = GameData::spr_button_Play_passive.sprite;
			sprite_dimensions.X = GameData::spr_button_Play_passive.width;
			sprite_dimensions.Y = GameData::spr_button_Play_passive.height;
		}
		else if (spriteType == "button_Quit_passive")
		{
			actorSprite = GameData::spr_button_Quit_passive.sprite;
			sprite_dimensions.X = GameData::spr_button_Quit_passive.width;
			sprite_dimensions.Y = GameData::spr_button_Quit_passive.height;
		}
		else if (spriteType == "button_Play_active")
		{
			actorSprite = GameData::spr_button_Play_active.sprite;
			sprite_dimensions.X = GameData::spr_button_Play_active.width;
			sprite_dimensions.Y = GameData::spr_button_Play_active.height;
		}
		else if (spriteType == "button_Quit_active")
		{
			actorSprite = GameData::spr_button_Quit_active.sprite;
			sprite_dimensions.X = GameData::spr_button_Quit_active.width;
			sprite_dimensions.Y = GameData::spr_button_Quit_active.height;
		}
		else if (spriteType == "highScore")
		{
			actorSprite = GameData::spr_highScore.sprite;
			sprite_dimensions.X = GameData::spr_highScore.width;
			sprite_dimensions.Y = GameData::spr_highScore.height;
		}
		else if (spriteType == "newBest")
		{
			actorSprite = GameData::spr_newBest.sprite;
			sprite_dimensions.X = GameData::spr_newBest.width;
			sprite_dimensions.Y = GameData::spr_newBest.height;
		}

		break;
	}
	case MENUSCORE:
	{
		if (digit >= 0)
		{
			actorSprite = GameData::menu_numbers.at(digit).sprite;
			sprite_dimensions.X = GameData::menu_numbers.at(digit).width;
			sprite_dimensions.Y = GameData::menu_numbers.at(digit).height;
		}

		break;
	}
	case PLATFORMS:
	{
		if (spriteType == "normal")
		{
			actorSprite = GameData::spr_platform.sprite;
			sprite_dimensions.X = GameData::spr_platform.width;
			sprite_dimensions.Y = GameData::spr_platform.height;
		}
		else if (spriteType == "timed")
		{
			actorSprite = GameData::spr_platform_timed.sprite;
			sprite_dimensions.X = GameData::spr_platform_timed.width;
			sprite_dimensions.Y = GameData::spr_platform_timed.height;
		}

		break;
	}
	case PLAYER:
	{
		actorSprite = GameData::spr_player.sprite;
		sprite_dimensions.X = GameData::spr_player.width;
		sprite_dimensions.Y = GameData::spr_player.height;
		break;
	}
	case PROJECTILES:
	{
		actorSprite = GameData::spr_projectile.sprite;
		sprite_dimensions.X = GameData::spr_projectile.width;
		sprite_dimensions.Y = GameData::spr_projectile.height;
		break;
	}
	case ENEMIES:
	{
		actorSprite = GameData::spr_enemy.sprite;
		sprite_dimensions.X = GameData::spr_enemy.width;
		sprite_dimensions.Y = GameData::spr_enemy.height;
		break;
	}
	case SCORE:
	{
		if (digit >= 0)
		{
			actorSprite = GameData::numbers.at(digit).sprite;
			sprite_dimensions.X = GameData::numbers.at(digit).width;
			sprite_dimensions.Y = GameData::numbers.at(digit).height;
		}
		else
		{
			actorSprite = GameData::spr_coin.sprite;
			sprite_dimensions.X = GameData::spr_coin.width;
			sprite_dimensions.Y = GameData::spr_coin.height;
		}
		break;
	}
	case SHIELD:
	{
		actorSprite = GameData::spr_shield.sprite;
		sprite_dimensions.X = GameData::spr_shield.width;
		sprite_dimensions.Y = GameData::spr_shield.height;
		break;
	}
	case COINS:
	{
		actorSprite = GameData::spr_coin.sprite;
		sprite_dimensions.X = GameData::spr_coin.width;
		sprite_dimensions.Y = GameData::spr_coin.height;
	}
	default:
	{
		//actorSprite = nullptr;
		break;
	}
	}
}

Sprite* Actor::GetSprite()
{
	return this->actorSprite;
}

void Actor::SetSpriteLocation(Vector2D location)
{
	this->sprite_location = location;
	SetColliderLocation(location);
}

Vector2D Actor::GetSpriteLocation()
{
	return this->sprite_location;
}

Vector2D Actor::GetSpriteDimensions()
{
	return sprite_dimensions;
}

void Actor::SetColliderLocation(Vector2D location)
{
	this->collider_location = location;
	collider.X = location.X;
	collider.Y = location.Y;
}

Vector2D Actor::GetColliderLocation()
{
	return collider_location;
}

Vector2D Actor::GetColliderDimensions()
{
	return collider_dimensions;
}

void Actor::SetColliderDimensions(int X, int Y)
{
	if (X && Y)
	{
		collider_dimensions.X = X;
		collider_dimensions.Y = Y;
	}

}

BoxCollider Actor::GetBoxCollider()
{
	return collider;
}

void Actor::Destroy()
{
	delete this;
}


