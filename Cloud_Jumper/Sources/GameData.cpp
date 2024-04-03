#include "GameData.h"
#define _CRT_SECURE_NO_WARNINGS

SpriteData::SpriteData(Sprite* sprite)
{
	this->sprite = sprite;
	width = 0;
	height = 0;
	if (sprite)
		getSpriteSize(sprite, width, height);
}

void SpriteData::Set(Sprite* sprite)
{
	this->sprite = sprite;
	width = 0;
	height = 0;
	if (sprite)
		getSpriteSize(sprite, width, height);
}


GameData::GameData()
	:lengthenScreen(false),
	lengthen_startCoord_Y(0)
{

}

bool GameData::Init()
{
	getScreenSize(screenWidth, screenHeight);

	spr_background.Set(createSprite(GetSpritePath("background.png").c_str()));
	spr_platform.Set(createSprite(GetSpritePath("platform_standard.png").c_str()));
	spr_platform_timed.Set(createSprite(GetSpritePath("platform_timed.png").c_str()));
	spr_player.Set(createSprite(GetSpritePath("cloud_player.png").c_str()));
	spr_projectile.Set(createSprite(GetSpritePath("projectile.png").c_str()));
	spr_enemy.Set(createSprite(GetSpritePath("cloud_evil.png").c_str()));
	spr_shield.Set(createSprite(GetSpritePath("shield.png").c_str()));
	spr_coin.Set(createSprite(GetSpritePath("star.png").c_str()));

	InitNumberSprites();

	SetDefaultSpawnRates(10, 30, 20);
	ScaleBackgroundToWindowSize();

	trackedScore = 0;
	trackedPlatforms = 0;

	return true;
}

void GameData::ScaleBackgroundToWindowSize()
{
	float ratio = (float)screenWidth / (float)spr_background.width;
	spr_background.width *= ratio;
	spr_background.height *= ratio;
	setSpriteSize(spr_background.sprite, spr_background.width, spr_background.height);

}

std::vector<std::pair<Sprite*, Vector2D>> GameData::GetBackgroundToDraw()
{
	std::vector<std::pair<Sprite*, Vector2D>> background_sprites;

	int totalHeight = 0;
	while (totalHeight < screenHeight)
	{
		background_sprites.push_back(std::make_pair(spr_background.sprite, Vector2D(0, totalHeight)));
		totalHeight += spr_background.height;
	}

	return background_sprites;
}

std::string GameData::GetSpritePath(std::string spriteName)
{
	std::string solutionDir = SOLUTION_DIR;
	std::string spriteFolderPath = "Sprites\\";
	std::string result = solutionDir + spriteFolderPath + spriteName;

	return result;
}

void GameData::InitNumberSprites()
{
	spr_number_0.Set(createSprite(GetSpritePath("number_0.png").c_str()));
	spr_number_1.Set(createSprite(GetSpritePath("number_1.png").c_str()));
	spr_number_2.Set(createSprite(GetSpritePath("number_2.png").c_str()));
	spr_number_3.Set(createSprite(GetSpritePath("number_3.png").c_str()));
	spr_number_4.Set(createSprite(GetSpritePath("number_4.png").c_str()));
	spr_number_5.Set(createSprite(GetSpritePath("number_5.png").c_str()));
	spr_number_6.Set(createSprite(GetSpritePath("number_6.png").c_str()));
	spr_number_7.Set(createSprite(GetSpritePath("number_7.png").c_str()));
	spr_number_8.Set(createSprite(GetSpritePath("number_8.png").c_str()));
	spr_number_9.Set(createSprite(GetSpritePath("number_9.png").c_str()));

	numbers.emplace(0, spr_number_0);
	numbers.emplace(1, spr_number_1);
	numbers.emplace(2, spr_number_2);
	numbers.emplace(3, spr_number_3);
	numbers.emplace(4, spr_number_4);
	numbers.emplace(5, spr_number_5);
	numbers.emplace(6, spr_number_6);
	numbers.emplace(7, spr_number_7);
	numbers.emplace(8, spr_number_8);
	numbers.emplace(9, spr_number_9);
}

void GameData::SetDefaultSpawnRates(int enemies, int timedPlatforms, int coins)
{
	spawnRate_enemy = MyMath::clamp(enemies, 0, 100);
	spawnRate_timedPlatform = MyMath::clamp(timedPlatforms, 0, 100);
	spawnRate_coin = MyMath::clamp(coins, 0, 100);
}



int GameData::screenWidth = 0;
int GameData::screenHeight = 0;

SpriteData GameData::spr_background(nullptr);
SpriteData GameData::spr_platform(nullptr);
SpriteData GameData::spr_platform_timed(nullptr);
SpriteData GameData::spr_player(nullptr);
SpriteData GameData::spr_projectile(nullptr);
SpriteData GameData::spr_enemy(nullptr);
SpriteData GameData::spr_shield(nullptr);
SpriteData GameData::spr_coin(nullptr);

SpriteData GameData::spr_number_0(nullptr);
SpriteData GameData::spr_number_1(nullptr);
SpriteData GameData::spr_number_2(nullptr);
SpriteData GameData::spr_number_3(nullptr);
SpriteData GameData::spr_number_4(nullptr);
SpriteData GameData::spr_number_5(nullptr);
SpriteData GameData::spr_number_6(nullptr);
SpriteData GameData::spr_number_7(nullptr);
SpriteData GameData::spr_number_8(nullptr);
SpriteData GameData::spr_number_9(nullptr);

unsigned int GameData::trackedScore = 0;
unsigned int GameData::trackedPlatforms = 0;

int GameData::spawnRate_enemy = 0;
int GameData::spawnRate_timedPlatform = 0;
int GameData::spawnRate_coin = 0;

std::unordered_map < int, SpriteData> GameData::numbers;