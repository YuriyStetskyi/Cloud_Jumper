#include "Game_Framework.h"


//inherited implementation

void Game_Framework::PreInit(int& width, int& height, bool& fullscreen) 
{
	width = screen_width;
	height = screen_height;
	fullscreen = screen_fullscreen;
}

bool Game_Framework::Init() 
{
	//constructor basically first
	srand(time(NULL));
	DeltaTime = 0;
	targetFPS = 144.0f;

	//initialization
	isMainMenu = true;
	gd.Init();
	player = new PlayerCharacter;

	saveFile = "saveFile.dat";
	LoadSaveFile();

	layer_background = gd.GetBackgroundToDraw();
	layer_mainMenu = GetMainMenuToDraw();
	layer_platforms = GeneratePlatforms((screen_width * screen_height) / 16000); //nice proportion of platforms/screenArea
	layer_player.clear();
	layer_player.push_back(player);
	layer_projectiles.clear();
	layer_enemies.clear();
	layer_shield.clear();
	layer_coins.clear();
	//layer_highScore.clear();

	gd.soundPaths.at(Sounds::AMBIENT_BIRDS)->setIsPaused(false);
	


	highScored = false;
	return true;
}

void Game_Framework::Close()
{
	closeGame = true;
}


bool Game_Framework::Tick()
{
	ComputeDeltaTime(DeltaTime);
	if (!closeGame)
	{
		std::chrono::steady_clock::time_point frameStart = std::chrono::steady_clock().now();

		dtSUM += DeltaTime;
		if (DeltaTime > highestThisSecond)
			highestThisSecond = DeltaTime;

		if (dtSUM > 1)
		{
			//std::cout << "lowest fps: " << 1 / highestThisSecond << std::endl;
			highestThisSecond = 0;
			dtSUM = 0;
		}

		if (isMainMenu)
		{
			HandleDrawing_Menu();
			HandleMainMenu();
		}
		else
		{

			HandleGameOver();
			HandlePlayerLogic();

			HandleObjects_Lifetime();
			CalculateScore();
			HandleScore_Pixels();
			HandleScore_Platforms();
			HandleScore_Coins();
			HandleTimedPlatforms();
			HandleShield();
			HandleDrawing();

		}
		drawer.Draw();
		drawer.Clear();
		std::chrono::steady_clock::time_point frameEnd = std::chrono::steady_clock().now();
		std::chrono::duration<float> elapsedTime = frameEnd - frameStart;
		LimitFPS(targetFPS, elapsedTime);
	
		return false;
	}
	else
	{

		SoundEngine->stopAllSounds();
		return true;
	}


}

void Game_Framework::onMouseMove(int x, int y, int xrelative, int yrelative)
{
	mouseX = x;
	mouseY = y;
}

void Game_Framework::onMouseButtonClick(FRMouseButton button, bool isReleased)
{

	if (button == FRMouseButton::LEFT && isReleased && isMainMenu)
	{
		if (layer_mainMenu[0] && layer_mainMenu[1])
		{
			if (layer_mainMenu[0]->GetBoxCollider().isHovered(Vector2D(mouseX, mouseY)))
			{
				isMainMenu = false;
			}
			else if (layer_mainMenu[1]->GetBoxCollider().isHovered(Vector2D(mouseX, mouseY)))
			{
				closeGame = true;
			}
		}
	}

	if (button == FRMouseButton::LEFT && !isReleased && !isMainMenu)
	{
		layer_projectiles.emplace(Actor::counter, new Actor(Vector2D(player->GetSpriteLocation().X, player->GetSpriteLocation().Y), PROJECTILES));
		layer_projectiles[Actor::counter]->projectileDestination = Vector2D(mouseX, mouseY);
		layer_projectiles[Actor::counter]->cashedPlayerLocation = player->GetSpriteLocation();
		
		SoundEngine->play2D(gd.soundPaths.at(Sounds::SOUND_SHOOT)->getSoundSource(), false);
	}

	if (button == FRMouseButton::RIGHT && !isReleased && layer_shield.empty() && player->coinsCollected >= 20)
	{
		layer_shield.emplace(Actor::counter, new Actor(Vector2D(player->GetSpriteLocation().X, player->GetSpriteLocation().Y), SHIELD));
		player->coinsCollected -= 20;

		//play sound
		SoundEngine->play2D(GameData::soundPaths.at(Sounds::SOUND_SHIELD_ENGAGE)->getSoundSource(), false);
		SoundEngine->play2D(GameData::soundPaths.at(Sounds::AMBIENT_SHIELD_5S)->getSoundSource(), false);
	}

	if (button == FRMouseButton::MIDDLE && !isReleased)
	{
		if (isMainMenu)
		{
			isMainMenu = false;
		}
		else
		{
			isMainMenu = true;
		}
	}
}

void Game_Framework::onKeyPressed(FRKey k)
{
	if (k == FRKey::RIGHT || k == FRKey::D)
	{
		player->rightInput = 1.0f;
		player->isMovingRight = true;
		player->isMovingLeft = false;
	}
	else if (k == FRKey::LEFT || k == FRKey::A)
	{
		player->rightInput = -1.0f;
		player->isMovingLeft = true;
		player->isMovingRight = false;
	}
	else if (k == FRKey::ESCAPE)
	{
		closeGame = true;
	}

}

void Game_Framework::onKeyReleased(FRKey k)
{
	if (k == FRKey::RIGHT || k == FRKey::D)
	{
		player->isMovingRight = false;
	}
	else if (k == FRKey::LEFT || k == FRKey::A)
	{
		player->isMovingLeft = false;
	}
}

const char* Game_Framework::GetTitle()
{
	return "Cloud Jumper";
}

Game_Framework::~Game_Framework()
{

}

void Game_Framework::ChangePreInitStats(int width, int height, bool fullscreen)
{

	screen_width = width;
	screen_height = height;
	screen_fullscreen = fullscreen;
}


//----------------------------------------------------------


void Game_Framework::ComputeDeltaTime(float& DeltaTime)
{
	previous_frame = current_frame;
	current_frame = std::chrono::high_resolution_clock::now();

	// divide by 1000000000 to convert from nanoseconds to seconds
	DeltaTime = ((float)(current_frame - previous_frame).count()) / 1000000000;
}

void Game_Framework::LimitFPS(float targetFPS, std::chrono::duration<float> elapsedTime)
{
	float elapsed = elapsedTime.count();
	//this should be the right implementation but for some reason this_thread::sleep_for is either
	//skipping or not working properly. its not a big deal anyway - i have delta time no matter what
	float targetFrameDuration = (1 / targetFPS);
	if (elapsed < targetFrameDuration)
	{
		float sleepDuration = (targetFrameDuration - elapsed) * 1000;

		std::chrono::milliseconds time = std::chrono::milliseconds((int)sleepDuration);
		std::this_thread::sleep_for(time);
	}
}

void Game_Framework::HandlePlayerLogic()
{
	player->MoveOnInput(player->rightInput, DeltaTime);
	player->JumpOnCollision(layer_platforms, DeltaTime);
	player->MoveCamera(layer_platforms, DeltaTime);
	player->MoveProjectiles(layer_projectiles, DeltaTime);
	player->MoveCamera(layer_enemies, DeltaTime);
	player->MoveCamera(layer_coins, DeltaTime);

}

void Game_Framework::HandleDrawing()
{
	drawer.AddToDraw<std::pair<Sprite*, Vector2D>>(layer_background, BACKGROUND);
	drawer.AddToDraw<Actor>(layer_platforms, PLATFORMS);
	drawer.AddToDraw<PlayerCharacter>(layer_player, PLAYER);
	drawer.AddToDraw<Actor>(layer_projectiles, PROJECTILES);
	drawer.AddToDraw<Actor>(layer_enemies, ENEMIES);
	drawer.AddToDraw<Actor>(layer_score, SCORE);
	drawer.AddToDraw<Actor>(layer_scorePlatforms, SCORE);
	drawer.AddToDraw<Actor>(layer_shield, SHIELD);
	drawer.AddToDraw<Actor>(layer_coins, COINS);
	drawer.AddToDraw<Actor>(layer_scoreCoins, SCORE);

}

void Game_Framework::HandleDrawing_Menu()
{
	drawer.AddToDraw<std::pair<Sprite*, Vector2D>>(layer_background, BACKGROUND);
	drawer.AddToDraw<Actor>(layer_mainMenu, MAINMENU);
	drawer.AddToDraw<Actor>(layer_highScore, SCORE);
}

void Game_Framework::HandleMainMenu()
{
	if (layer_mainMenu[0]->GetBoxCollider().isHovered(Vector2D(mouseX, mouseY))
		&& layer_mainMenu[0]->spriteType != "button_Play_active")
	{
		layer_mainMenu[0] = new Actor(Vector2D(100, screen_height - 750), "button_Play_active", MAINMENU);
	}

	if (layer_mainMenu[1]->GetBoxCollider().isHovered(Vector2D(mouseX, mouseY))
		&& layer_mainMenu[0]->spriteType != "button_Quit_active")
	{
		layer_mainMenu[1] = new Actor(Vector2D(100, screen_height - 500), "button_Quit_active", MAINMENU);
	}

	if (!layer_mainMenu[0]->GetBoxCollider().isHovered(Vector2D(mouseX, mouseY))
		&& layer_mainMenu[0]->spriteType != "button_Play_passive")
	{
		layer_mainMenu[0] = new Actor(Vector2D(100, screen_height - 750), "button_Play_passive", MAINMENU);
	}

	if (!layer_mainMenu[1]->GetBoxCollider().isHovered(Vector2D(mouseX, mouseY))
		&& layer_mainMenu[0]->spriteType != "button_Quit_passive")
	{
		layer_mainMenu[1] = new Actor(Vector2D(100, screen_height - 500), "button_Quit_passive", MAINMENU);
	}

}

std::unordered_map<int, Actor*> Game_Framework::GetMainMenuToDraw()
{
	std::unordered_map<int, Actor*> mMenu;
	mMenu.emplace(0, new Actor(Vector2D(100, screen_height - 750), "button_Play_passive", MAINMENU));
	mMenu.emplace(1, new Actor(Vector2D(100, screen_height - 500), "button_Quit_passive", MAINMENU));
	mMenu.emplace(2, new Actor(Vector2D(100, screen_height - 250), "highScore", MAINMENU));

	if (highScored)
	{
		mMenu.emplace(3, new Actor(Vector2D(100, screen_height - 250), "newBest", MAINMENU));
	}
		
	
	return mMenu;
}

std::unordered_map<int, Actor*> Game_Framework::GeneratePlatforms(int count)
{
	std::unordered_map<int, Actor*> platforms;

	Vector2D platformSpawnLocation;

	bool restartLoop = false;
	platformSpawnLocation = Vector2D(MyMath::randInRange(0, screen_width - GameData::spr_platform.width), MyMath::randInRange(0, screen_height));

	if (platforms.empty())
	{
		platforms.emplace(Actor::counter, new Actor(platformSpawnLocation, "normal", PLATFORMS));
	}
	for (int i = 0; i < count - 1; i++)
	{
		do
		{
			restartLoop = false;
			for (std::pair<int, Actor*> platform : platforms)
			{
				bool collided = BoxCollider::AreColliding(BoxCollider(Vector2D(platformSpawnLocation.X, platformSpawnLocation.Y), Vector2D(platform.second->GetSpriteDimensions().X, platform.second->GetSpriteDimensions().Y)), platform.second->GetBoxCollider());
				if (collided)
				{
					platformSpawnLocation = Vector2D(MyMath::randInRange(0, screen_width - GameData::spr_platform.width), MyMath::randInRange(0, screen_height));
					restartLoop = true;
					break;
				}
			}

		} while (restartLoop);

		platforms.emplace(Actor::counter, new Actor(platformSpawnLocation, "normal", PLATFORMS));
	}

	return platforms;

}

void Game_Framework::HandleObjects_Lifetime()
{
	std::unordered_map<int, Actor*> temp_platforms = layer_platforms;
	std::unordered_map<int, Actor*> temp_enemies = layer_enemies;
	std::unordered_map<int, Actor*> temp_projectiles = layer_projectiles;
	std::unordered_map<int, Actor*> temp_coins = layer_coins;


	HandleObjects_Spawn(temp_platforms, temp_enemies, temp_projectiles, temp_coins);
	HandleObjects_Despawn(temp_platforms, temp_enemies, temp_projectiles, temp_coins);
}

void Game_Framework::HandleObjects_Spawn(std::unordered_map<int, Actor*>& temp_platforms,
	std::unordered_map<int, Actor*>& temp_enemies,
	std::unordered_map<int, Actor*>& temp_projectiles,
	std::unordered_map<int, Actor*>& temp_coins)
{
	Vector2D platformSpawnLocation;
	for (std::pair<int, Actor*> platform : layer_platforms)
	{
		if (platform.second->GetSpriteLocation().Y > screen_height)
		{
			temp_platforms.erase(platform.first);

			bool restartLoop = false;
			platformSpawnLocation = Vector2D(MyMath::randInRange(0, screen_width - GameData::spr_platform.width), MyMath::randInRange(-75, 0));
			do
			{
				restartLoop = false;
				for (std::pair<int, Actor*> platform : temp_platforms)
				{
					bool collided = BoxCollider::AreColliding(BoxCollider(Vector2D(platformSpawnLocation.X, platformSpawnLocation.Y), Vector2D(platform.second->GetSpriteDimensions().X, platform.second->GetSpriteDimensions().Y)), platform.second->GetBoxCollider());
					if (collided)
					{
						platformSpawnLocation = Vector2D(MyMath::randInRange(0, screen_width - GameData::spr_platform.width), MyMath::randInRange(-75, 0));
						restartLoop = true;
						break;
					}
				}
			} while (restartLoop);


			if (MyMath::randInRange(0, 100) < GameData::spawnRate_timedPlatform)
			{
				temp_platforms.emplace(Actor::counter, new Actor(platformSpawnLocation, "timed", PLATFORMS));
			}
			else
			{
				temp_platforms.emplace(Actor::counter, new Actor(platformSpawnLocation, "normal", PLATFORMS));
			}


			if (MyMath::randInRange(0, 100) < GameData::spawnRate_enemy)
				temp_enemies.emplace(Actor::counter, new Actor(Vector2D(platformSpawnLocation.X - 15, platformSpawnLocation.Y - 60), ENEMIES));

			if (MyMath::randInRange(0, 100) < GameData::spawnRate_coin)
				temp_coins.emplace(Actor::counter, new Actor(Vector2D(platformSpawnLocation.X + 17, platformSpawnLocation.Y - 30), COINS));
		}

	}
}

void Game_Framework::HandleObjects_Despawn(std::unordered_map<int, Actor*>& temp_platforms,
	std::unordered_map<int, Actor*>& temp_enemies,
	std::unordered_map<int, Actor*>& temp_projectiles,
	std::unordered_map<int, Actor*>& temp_coins)
{
	for (std::pair<int, Actor*> enemy : layer_enemies)
	{
		for (std::pair<int, Actor*> projectile : layer_projectiles)
		{
			if (BoxCollider::AreColliding(enemy.second->collider, projectile.second->collider))
			{
				temp_enemies.erase(enemy.first);
				temp_projectiles.erase(projectile.first);
			}
		}

		if (enemy.second->GetSpriteLocation().Y > screen_height)
		{
			temp_enemies.erase(enemy.first);

		}
	}

	for (std::pair<int, Actor*> coin : layer_coins)
	{
		if (BoxCollider::AreColliding(coin.second->collider, player->collider_enemy))
		{
			temp_coins.erase(coin.first);
			player->coinsCollected++;

			//play sound
			SoundEngine->play2D(GameData::soundPaths.at(Sounds::SOUND_STAR_1)->getSoundSource(), false);
		}
	}

	layer_platforms = temp_platforms;
	layer_enemies = temp_enemies;
	layer_projectiles = temp_projectiles;
	layer_coins = temp_coins;
}

void Game_Framework::HandleGameOver()
{
	if (player->GetSpriteLocation().Y > (screen_height)) //player fell
	{
		//restart game
		SaveHighScore();
		Init();
	}

	std::unordered_map<int, Actor*> temp_enemies = layer_enemies;
	for (std::pair<int, Actor*> enemy : layer_enemies)
	{
		bool hitEnemy = BoxCollider::AreColliding(player->collider_enemy, enemy.second->collider);

		if (hitEnemy && player->isJumping && !player->isImmune) //player hit enemy
		{
			//restart game
			SaveHighScore();
			Init();
			return;
		}
		else if (hitEnemy && !player->isJumping)
		{
			temp_enemies.erase(enemy.first);
		}
	}
	layer_enemies = temp_enemies;
}

void Game_Framework::CalculateScore()
{

	for (std::pair<int, Actor*> platform : layer_platforms)
	{

		if (platform.second->GetSpriteLocation().Y < player->GetSpriteLocation().Y)
		{
			if (!platform.second->passedByPlayer)
			{
				GameData::trackedPlatforms++;
				platform.second->passedByPlayer = true;
			}
		}

	}
}

void Game_Framework::HandleScore_Pixels()
{
	layer_score.clear();
	int l_score = GameData::trackedScore;
	int position = 0;

	while (l_score > 0)
	{
		int digit = l_score % 10;

		layer_score.push_back(new Actor(Vector2D(position * 0, 0), digit, SCORE));
		position++;
		l_score /= 10;
	}
	std::reverse(layer_score.begin(), layer_score.end());

	int position_afterReverse = 0;
	if (!layer_score.empty())
	{
		for (Actor* digit : layer_score)
		{
			digit->SetSpriteLocation(Vector2D(position_afterReverse * 25, 10));
			position_afterReverse++;
		}
	}
	else
	{
		layer_score.push_back(new Actor(Vector2D(0, 10), 0, SCORE));
	}

}

void Game_Framework::HandleScore_Platforms()
{
	layer_scorePlatforms.clear();
	int l_platforms = GameData::trackedPlatforms;
	int position = 0;

	while (l_platforms > 0)
	{
		int digit = l_platforms % 10;


		layer_scorePlatforms.push_back(new Actor(Vector2D(0, 0), digit, SCORE));
		position++;
		l_platforms /= 10;
	}
	std::reverse(layer_scorePlatforms.begin(), layer_scorePlatforms.end());

	int position_afterReverse = 0;
	if (!layer_scorePlatforms.empty())
	{
		for (Actor* digit : layer_scorePlatforms)
		{
			digit->SetSpriteLocation(Vector2D(position_afterReverse * 25, 50));
			position_afterReverse++;
		}
	}
	else
	{
		layer_scorePlatforms.push_back(new Actor(Vector2D(0, 50), 0, SCORE));
	}
}

void Game_Framework::HandleTimedPlatforms()
{
	std::unordered_map<int, Actor*> temp_layer_platforms = layer_platforms;
	for (std::pair<int, Actor*> platform : layer_platforms)
	{
		if (platform.second->toBeDestroyed)
		{
			if (platform.second->timeAfterSteppedOn < 0.5)
			{
				platform.second->timeAfterSteppedOn += DeltaTime;
			}
			else
				temp_layer_platforms.erase(platform.first);
		}
	}
	layer_platforms = temp_layer_platforms;
}

void Game_Framework::HandleShield()
{
	bool shieldOver = false;
	for (std::pair<int, Actor*> shield : layer_shield)
	{
		shield.second->timeAfterShieldUsed += DeltaTime;
		shield.second->SetSpriteLocation(player->GetSpriteLocation());
		if (shield.second->timeAfterShieldUsed > 5)
			shieldOver = true;
	}

	if (shieldOver)
		layer_shield.clear();

	if (!layer_shield.empty())
	{
		player->isImmune = true;
	}
	else
	{
		player->isImmune = false;
	}


}


void Game_Framework::HandleScore_Coins()
{
	layer_scoreCoins.clear();
	int coins = player->coinsCollected;
	int position = 0;

	while (coins > 0)
	{
		int digit = coins % 10;

		

		layer_scoreCoins.push_back(new Actor(Vector2D(0, 0), digit, SCORE));
		position++;
		coins /= 10;
	}
	std::reverse(layer_scoreCoins.begin(), layer_scoreCoins.end());

	int position_afterReverse = 0;
	if (!layer_scoreCoins.empty())
	{
		for (Actor* digit : layer_scoreCoins)
		{
			digit->SetSpriteLocation(Vector2D(position_afterReverse * 25, 90));
			position_afterReverse++;
		}
		layer_scoreCoins.push_back(new Actor(Vector2D(position_afterReverse * 30, 90), -1, SCORE));
	}
	else
	{
		layer_scoreCoins.push_back(new Actor(Vector2D(0, 90), 0, SCORE));
		layer_scoreCoins.push_back(new Actor(Vector2D(30, 90), -1, SCORE));
	}
}

void Game_Framework::SaveHighScore()
{
	if (GameData::trackedScore > GameData::trackedHighScore)
	{
		highScored = true;
		layer_highScore = layer_score;
		GameData::trackedHighScore = GameData::trackedScore;
		for (Actor* item : layer_highScore)
		{
			Vector2D locationInUI(item->GetSpriteLocation().X + 650, item->GetSpriteLocation().Y + screen_height - 150);
			item->SetSpriteLocation(locationInUI);
		}

		std::fstream output;
		output.open(saveFile, std::fstream::out);

		if (output.is_open())
		{
			//write highscore (int)
			output.write((char*)(&GameData::trackedHighScore), sizeof(unsigned int));
		}

		output.close();
	}
}

void Game_Framework::LoadSaveFile()
{
	layer_highScore;
	GameData::trackedHighScore;

	std::fstream input;
	input.open(saveFile, std::fstream::in);
	
	//read highscore number
	unsigned int scr = 0;
	
	while (input.read((char*)(&scr), sizeof(unsigned int)))
	{
		GameData::trackedHighScore = scr;
	}

	GameData::trackedScore = GameData::trackedHighScore; //do this for next method to work
	HandleScore_Pixels();
	layer_highScore = layer_score;
	for (Actor* item : layer_highScore)
	{
		Vector2D locationInUI(item->GetSpriteLocation().X + 650, item->GetSpriteLocation().Y + screen_height - 150);
		item->SetSpriteLocation(locationInUI);
	}

	GameData::trackedScore = 0;
	input.close();

}
