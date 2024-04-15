#pragma once
#include "GLFW_Framework.h"
#include <vector>
#include "MyMath.h"
#include "Actor.h"
#include "PlayerCharacter.h"
#include <unordered_map>
#include "MyEnums.h"



class Drawer
{
public:

	std::vector < std::pair<Sprite*, Vector2D> >layer_background;

	std::vector < std::pair<Sprite*, Vector2D> >layer_mainMenu;

	std::vector < std::pair<Sprite*, Vector2D> >layer_menuScore;

	std::vector < std::pair<Sprite*, Vector2D> >layer_player;

	std::vector < std::pair<Sprite*, Vector2D> >layer_platforms;

	std::vector < std::pair<Sprite*, Vector2D> >layer_projectiles;

	std::vector < std::pair<Sprite*, Vector2D> >layer_enemies;

	std::vector < std::pair<Sprite*, Vector2D> >layer_score;

	std::vector < std::pair<Sprite*, Vector2D> >layer_shield;

	std::vector < std::pair<Sprite*, Vector2D> >layer_coins;

	Drawer();
	void Draw();
	void Draw_Layer(DrawLayer layer);
	void Clear();


	template <typename T>
	void AddToDraw(std::vector<T*> items, DrawLayer layer);

	//non-pointer version for background (crutch)
	template <typename T>
	void AddToDraw(std::vector<T> items, DrawLayer layer);

	template<typename T>
	void AddToDraw(std::unordered_map<int, T*> items, DrawLayer layer);




};

