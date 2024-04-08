#pragma once

#include "GLFW_Framework.h"
#include <vector>
#include "Drawer.h"
#include "GameData.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "MyMath.h"
#include "PlayerCharacter.h"
#include <string>
#include "Actor.h"
#include <algorithm>
#include <unordered_map>
//#include "Platform.h"
#include "MyEnums.h"


class Game_Framework : public GLFW_Framework
{
public:

	//inherited implementation
	virtual void PreInit(int& width, int& height, bool& fullscreen) override;
	virtual bool Init() override;
	virtual void Close() override;
	virtual bool Tick() override;
	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) override;
	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) override;
	virtual void onKeyPressed(FRKey k) override;
	virtual void onKeyReleased(FRKey k) override;
	virtual const char* GetTitle() override;

	virtual ~Game_Framework();
	//------------------------------

	//preInit
	void ChangePreInitStats(int width, int height, bool fullscreen);

	bool screen_fullscreen = false;

	//close condition
	bool closeGame;

	//deltatime calculation + fps limiter
	float DeltaTime;
	void ComputeDeltaTime(float& DeltaTime);
	std::chrono::steady_clock::time_point previous_frame;
	std::chrono::steady_clock::time_point current_frame;
	float targetFPS;
	void LimitFPS(float targetFPS, std::chrono::duration<float> elapsedTime);

	//main gameloop implementation
	GameData gd;

	//drawing
	Drawer drawer;
	std::vector < std::pair<Sprite*, Vector2D> >layer_background;
	std::unordered_map<int, Actor*> layer_platforms;
	std::vector < PlayerCharacter* >layer_player;
	std::unordered_map<int, Actor*> layer_projectiles;
	std::unordered_map<int, Actor*> layer_enemies;
	std::unordered_map<int, Actor*> layer_coins;
	std::vector<Actor*> layer_score;
	std::vector<Actor*> layer_scorePlatforms;
	std::unordered_map<int, Actor*> layer_shield;
	std::vector<Actor*> layer_scoreCoins;

	std::unordered_map<int, Actor*>GeneratePlatforms(int count);
	void HandlePlayerLogic();
	void HandleDrawing();
	//player
	PlayerCharacter* player;

	//handle platforms spawning and despawning
	void HandleObjects_Lifetime();
	void HandleObjects_Spawn(std::unordered_map<int, Actor*>& temp_platforms,
		std::unordered_map<int, Actor*>& temp_enemies,
		std::unordered_map<int, Actor*>& temp_projectiles,
		std::unordered_map<int, Actor*>& temp_coinss);
	void HandleObjects_Despawn(std::unordered_map<int, Actor*>& temp_platforms,
		std::unordered_map<int, Actor*>& temp_enemies,
		std::unordered_map<int, Actor*>& temp_projectiles,
		std::unordered_map<int, Actor*>& temp_coins);

	//handling gameOver
	void HandleGameOver();

	//fps testing
	float highestThisSecond = 0;
	float dtSUM = 0;

	//mouse
	int mouseX;
	int mouseY;

	//handling score
	void CalculateScore();
	std::unordered_map<int, Actor*> PassedPlatforms;


	//handling score
	void HandleScore_Pixels();

	//handling platfroms display

	void HandleScore_Platforms();


	void HandleTimedPlatforms();
	void HandleShield();

	void HandleScore_Coins();

};





