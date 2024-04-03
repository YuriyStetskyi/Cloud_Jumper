#pragma once

#include "GLFW_Framework.h"
#include "MyMath.h"
#include <vector>
#include <string>
#include <unordered_map>

class GameData;

struct SpriteData
{
	SpriteData(Sprite* sprite);
	Sprite* sprite;
	int width;
	int height;
private:
	friend GameData;
	void Set(Sprite* sprite);
};

class GameData
{
public:
	GameData();
	bool Init();
	void ScaleBackgroundToWindowSize();
	std::vector<std::pair<Sprite*, Vector2D>> GetBackgroundToDraw();

	static int screenWidth;
	static int screenHeight;

	static SpriteData spr_background;
	static SpriteData spr_platform;
	static SpriteData spr_platform_timed;
	static SpriteData spr_player;
	static SpriteData spr_projectile;
	static SpriteData spr_enemy;
	static SpriteData spr_shield;
	static SpriteData spr_coin;

	static SpriteData spr_number_0;
	static SpriteData spr_number_1;
	static SpriteData spr_number_2;
	static SpriteData spr_number_3;
	static SpriteData spr_number_4;
	static SpriteData spr_number_5;
	static SpriteData spr_number_6;
	static SpriteData spr_number_7;
	static SpriteData spr_number_8;
	static SpriteData spr_number_9;

	static std::unordered_map < int, SpriteData> numbers;

	bool lengthenScreen;
	int lengthen_startCoord_Y;

	//score
	static unsigned int trackedScore;
	static unsigned int trackedPlatforms;

	static int spawnRate_enemy;
	static int spawnRate_timedPlatform;
	static int spawnRate_coin;

private:
	//input sprite name e.g: "spriteName.png"
	//Sprite must be in SolutionDir\Sprites
	std::string GetSpritePath(std::string spriteName);

	void InitNumberSprites();

	//In order: enemies, timedPlatforms, coins. From 0 to 100 (%)
	void SetDefaultSpawnRates(int enemies, int timedPlatforms, int coins);

};

