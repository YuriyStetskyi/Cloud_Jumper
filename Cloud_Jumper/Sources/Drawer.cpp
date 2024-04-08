#include "Drawer.h"

Drawer::Drawer()
{

}

void Drawer::Draw()
{
	//order matters
	Draw_Layer(BACKGROUND);
	Draw_Layer(MAINMENU);
	Draw_Layer(PLATFORMS);
	Draw_Layer(COINS);
	Draw_Layer(ENEMIES);
	Draw_Layer(SCORE);
	Draw_Layer(PLAYER);
	Draw_Layer(SHIELD);
	Draw_Layer(PROJECTILES);
}

void Drawer::Draw_Layer(DrawLayer layer)
{
	switch (layer)
	{
	case BACKGROUND:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_background)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
		break;
	}
	case MAINMENU:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_mainMenu)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
		break;
	}
	case PLATFORMS:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_platforms)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
		break;
	}
	case PLAYER:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_player)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
		break;
	}
	case PROJECTILES:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_projectiles)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
	}
	case ENEMIES:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_enemies)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
	}
	case SCORE:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_score)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
	}
	case SHIELD:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_shield)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
	}
	case COINS:
	{
		for (std::pair<Sprite*, Vector2D> item : layer_coins)
		{
			drawSprite(item.first, item.second.X, item.second.Y);
		}
	}
	default:
		break;
	}
}

void Drawer::Clear()
{
	layer_background.clear();
	layer_mainMenu.clear();
	layer_player.clear();
	layer_platforms.clear();
	layer_projectiles.clear();
	layer_enemies.clear();
	layer_score.clear();
	layer_shield.clear();
	layer_coins.clear();
}

template<typename T>
void Drawer::AddToDraw(std::vector<T*> items, DrawLayer layer)
{
	//default
}



template<>
void Drawer::AddToDraw<PlayerCharacter>(std::vector<PlayerCharacter*> items, DrawLayer layer)
{
	if (layer == PLAYER)
	{
		for (PlayerCharacter* player : items)
		{
			if (player)
				layer_player.push_back(std::make_pair(player->GetSprite(), player->GetSpriteLocation()));
		}
	}
}

template<>
void Drawer::AddToDraw<Actor>(std::vector<Actor*> items, DrawLayer layer)
{
	if (layer == SCORE)
	{
		for (Actor* digit : items)
		{
			layer_score.push_back(std::make_pair(digit->GetSprite(), digit->GetSpriteLocation()));
		}
	}
}


template<typename T>
void Drawer::AddToDraw(std::vector<T> items, DrawLayer layer)
{

}

template<>
void Drawer::AddToDraw<std::pair< Sprite*, Vector2D>>(std::vector<std::pair< Sprite*, Vector2D>> items, DrawLayer layer)
{
	if (layer == BACKGROUND)
	{
		for (std::pair< Sprite*, Vector2D> item : items)
		{
			layer_background.push_back(item);
		}
	}
}

template<typename T>
void Drawer::AddToDraw(std::unordered_map<int, T*> items, DrawLayer layer)
{

}

template<>
void Drawer::AddToDraw<Actor>(std::unordered_map<int, Actor*> items, DrawLayer layer)
{
	switch (layer)
	{
	case MAINMENU:
	{
		for (std::pair<int, Actor*> UIelement : items)
		{
			if (UIelement.second)
				layer_mainMenu.push_back(std::make_pair(UIelement.second->GetSprite(), UIelement.second->GetSpriteLocation()));
		}
	}
	case PLATFORMS:
	{
		for (std::pair<int, Actor*> platform : items)
		{
			if (platform.second)
				layer_platforms.push_back(std::make_pair(platform.second->GetSprite(), platform.second->GetSpriteLocation()));
		}
		break;
	}
	case PROJECTILES:
	{
		for (std::pair<int, Actor*> projectile : items)
		{
			if (projectile.second)
				layer_projectiles.push_back(std::make_pair(projectile.second->GetSprite(), projectile.second->GetSpriteLocation()));
		}
		break;
	}
	case ENEMIES:
	{
		for (std::pair<int, Actor*> enemy : items)
		{
			if (enemy.second)
				layer_enemies.push_back(std::make_pair(enemy.second->GetSprite(), enemy.second->GetSpriteLocation()));
		}
		break;
	}
	case SCORE:
	{
		for (std::pair<int, Actor*> number : items)
		{
			if (number.second)
				layer_score.push_back(std::make_pair(number.second->GetSprite(), number.second->GetSpriteLocation()));
		}
		break;
	}
	case SHIELD:
	{
		for (std::pair<int, Actor*> shield : items)
		{
			if (shield.second)
				layer_shield.push_back(std::make_pair(shield.second->GetSprite(), shield.second->GetSpriteLocation()));
		}
		break;
	}
	case COINS:
	{
		for (std::pair<int, Actor*> coin : items)
		{
			if (coin.second)
				layer_coins.push_back(std::make_pair(coin.second->GetSprite(), coin.second->GetSpriteLocation()));
		}
		break;
	}
	default:
		break;
	}
}


