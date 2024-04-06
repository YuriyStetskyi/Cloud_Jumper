#include "GameData.h"
#define _CRT_SECURE_NO_WARNINGS

#define DEBUG

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

	InitSprites();
	InitNumberSprites();
	InitSounds();

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

std::string GameData::GetSoundPath(std::string soundName)
{
	std::string solutionDir = SOLUTION_DIR;
	std::string spriteFolderPath = "Sounds\\";
	std::string result = solutionDir + spriteFolderPath + soundName;

	return result;
}

void GameData::InitSprites()
{
#ifdef DEBUG
	spr_background.Set(createSprite(GetSpritePath("background.png").c_str()));
	spr_platform.Set(createSprite(GetSpritePath("platform_standard.png").c_str()));
	spr_platform_timed.Set(createSprite(GetSpritePath("platform_timed.png").c_str()));
	spr_player.Set(createSprite(GetSpritePath("cloud_player.png").c_str()));
	spr_projectile.Set(createSprite(GetSpritePath("projectile.png").c_str()));
	spr_enemy.Set(createSprite(GetSpritePath("cloud_evil.png").c_str()));
	spr_shield.Set(createSprite(GetSpritePath("shield.png").c_str()));
	spr_coin.Set(createSprite(GetSpritePath("star.png").c_str()));
#endif // DEBUG

#ifndef DEBUG
	spr_background.Set(createSprite("Sprites\\background.png"));
	spr_platform.Set(createSprite("Sprites\\platform_standard.png"));
	spr_platform_timed.Set(createSprite("Sprites\\platform_timed.png"));
	spr_player.Set(createSprite("Sprites\\cloud_player.png"));
	spr_projectile.Set(createSprite("Sprites\\projectile.png"));
	spr_enemy.Set(createSprite("Sprites\\cloud_evil.png"));
	spr_shield.Set(createSprite("Sprites\\shield.png"));
	spr_coin.Set(createSprite("Sprites\\star.png"));
#endif // !DEBUG


}

void GameData::InitNumberSprites()
{
#ifdef DEBUG
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
#endif // DEBUG

#ifndef DEBUG
	spr_number_0.Set(createSprite("Sprites\\number_0.png"));
	spr_number_1.Set(createSprite("Sprites\\number_1.png"));
	spr_number_2.Set(createSprite("Sprites\\number_2.png"));
	spr_number_3.Set(createSprite("Sprites\\number_3.png"));
	spr_number_4.Set(createSprite("Sprites\\number_4.png"));
	spr_number_5.Set(createSprite("Sprites\\number_5.png"));
	spr_number_6.Set(createSprite("Sprites\\number_6.png"));
	spr_number_7.Set(createSprite("Sprites\\number_7.png"));
	spr_number_8.Set(createSprite("Sprites\\number_8.png"));
	spr_number_9.Set(createSprite("Sprites\\number_9.png"));
#endif // !DEBUG



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

void GameData::InitSounds()
{	
	//soundPaths.emplace(Sounds::AMBIENT_BIRDS, GLFW_Framework::SoundEngine->play2D("..\\Sounds\\birds.mp3", true, true));

#ifdef DEBUG
	soundPaths.emplace(Sounds::AMBIENT_BIRDS, GLFW_Framework::SoundEngine->play2D(GetSoundPath("birds.mp3").c_str(), true, true));
	soundPaths.at(Sounds::AMBIENT_BIRDS)->setVolume(0.1);
	soundPaths.emplace(Sounds::AMBIENT_SHIELD, GLFW_Framework::SoundEngine->play2D(GetSoundPath("shield_ambient.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::AMBIENT_SHIELD_5S, GLFW_Framework::SoundEngine->play2D(GetSoundPath("shield_ambient_5sec.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_JUMP, GLFW_Framework::SoundEngine->play2D(GetSoundPath("jump.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_SHIELD_ENGAGE, GLFW_Framework::SoundEngine->play2D(GetSoundPath("shield_engage.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_SHIELD_DISENGAGE, GLFW_Framework::SoundEngine->play2D(GetSoundPath("shield_disengage.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_SHOOT, GLFW_Framework::SoundEngine->play2D(GetSoundPath("shoot_sound.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_STAR_1, GLFW_Framework::SoundEngine->play2D(GetSoundPath("star_pickup_1.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_STAR_2, GLFW_Framework::SoundEngine->play2D(GetSoundPath("star_pickup_2.mp3").c_str(), false, true));
	soundPaths.emplace(Sounds::SOUND_STAR_3, GLFW_Framework::SoundEngine->play2D(GetSoundPath("star_pickup_3.mp3").c_str(), false, true));
#endif // DEBUG

#ifndef DEBUG
	soundPaths.emplace(Sounds::AMBIENT_BIRDS, GLFW_Framework::SoundEngine->play2D("Sounds\\birds.mp3", true, true));
	soundPaths.at(Sounds::AMBIENT_BIRDS)->setVolume(0.1);
	soundPaths.emplace(Sounds::AMBIENT_SHIELD, GLFW_Framework::SoundEngine->play2D("Sounds\\shield_ambient.mp3", false, true));
	soundPaths.emplace(Sounds::AMBIENT_SHIELD_5S, GLFW_Framework::SoundEngine->play2D("Sounds\\shield_ambient_5sec.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_JUMP, GLFW_Framework::SoundEngine->play2D("Sounds\\jump.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_SHIELD_ENGAGE, GLFW_Framework::SoundEngine->play2D("Sounds\\shield_engage.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_SHIELD_DISENGAGE, GLFW_Framework::SoundEngine->play2D("Sounds\\shield_disengage.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_SHOOT, GLFW_Framework::SoundEngine->play2D("Sounds\\shoot_sound.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_STAR_1, GLFW_Framework::SoundEngine->play2D("Sounds\\star_pickup_1.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_STAR_2, GLFW_Framework::SoundEngine->play2D("Sounds\\star_pickup_2.mp3", false, true));
	soundPaths.emplace(Sounds::SOUND_STAR_3, GLFW_Framework::SoundEngine->play2D("Sounds\\star_pickup_3.mp3", false, true));
#endif // !DEBUG






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

std::unordered_map<Sounds, ISound*> GameData::soundPaths;