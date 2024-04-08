#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stb/stb_image_resize2.h>
#include <stb/stb_image_write.h>
#include <irrklang/irrKlang.h>

#include "Shader.h"
#include <iostream>
#include <vector>
#include <map>

#define DEBUG

using namespace irrklang;

extern int GLOBAL_SCREEN_WIDTH;
extern int GLOBAL_SCREEN_HEIGHT;
extern int GLOBAL_FULLSCREEN;



// NEED
class Sprite
{
public:
	Sprite();
	~Sprite();
	
	unsigned int textureObject;
	int width;
	int height;
	float ndcWidth;
	float ndcHeight;
	int numberOfChannels;
	unsigned char* textureData;

	float X;
	float Y;
	//normalized device coordinates ( -1 : 1 ) NDC
	float ndcX;
	float ndcY;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

// NEED ------ DONE ------
Sprite* createSprite(const char* path);

// NEED ------ DONE ------
void drawSprite(Sprite*, int x, int y);

// NEED ------ DONE ------
void getSpriteSize(Sprite* s, int& w, int& h);

// NEED ------ DONE ------
void setSpriteSize(Sprite* s, int w, int h);

// NEED ------ DONE ------
void destroySprite(Sprite* s);	

// -
void drawTestBackground();

// NEED ------ DONE ------
void getScreenSize(int& w, int& h);

// NEED
//number of milliseconds since library initialization.
unsigned int getTickCount();

// -
void showCursor(bool bShow);



// NEED ------ DONE ------
enum class FRKey {
	RIGHT,
	LEFT,
	DOWN,
	UP,
	A,
	D,
	ESCAPE,
	COUNT
};

// NEED ------ DONE ------
enum class FRMouseButton {
	LEFT,
	MIDDLE,
	RIGHT,
	COUNT
};

class GLFW_Framework
{
public:
	//input processing
	void processInput(GLFWwindow* window);
	std::map<FRKey, bool> keys_pressed;
	std::map<FRMouseButton, bool> mouseButtons_pressed;

	//callbacks for virtual function
	void setTitle();
	void runPreInit(int& width, int& height, bool& fullscreen);
	bool runInit(GLFW_Framework* framework);
	void runClose();
	bool runTick();

	//pure virtual functions:

	virtual void PreInit(int& width, int& height, bool& fullscreen);
	// return : true - ok, false - failed, application will exit
	virtual bool Init();
	virtual void Close();
	// return value: if true will exit the application
	virtual bool Tick();
	// param: xrel, yrel: The relative motion in the X/Y direction 
	// param: x, y : coordinate, relative to window
	virtual void onMouseMove(int x, int y, int xrelative, int yrelative);
	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);
	virtual void onKeyPressed(FRKey k);
	virtual void onKeyReleased(FRKey k);
	virtual const char* GetTitle();
	virtual ~GLFW_Framework() {};

	//preInit
	//void ChangePreInitStats(int width, int height, bool fullscreen);
	int screen_width;
	int screen_height;
	bool screen_fullscreen = false;
	std::string title;	

	//window
	GLFWwindow* window;

	//monitor
	GLFWmonitor* monitor;

	//mode
	const GLFWvidmode* mode;

	//shader
	Shader shader;
	std::string vertexShader_path = PROJECT_DIR + std::string("Shaders\\VertexShader.vs");
	std::string fragmentShader_path = PROJECT_DIR + std::string("Shaders\\FragmentShader.fs");

	std::string vertexShader_RELEASEpath = "Shaders\\VertexShader.vs";
	std::string fragmentShader_RELEASEpath = "Shaders\\FragmentShader.fs";

	//sounds
	static ISoundEngine* SoundEngine;
	
};

void calculateSpriteVertices(Sprite* sprite, float* vertices, int arrSize);

//converts your coordinate to normalized device coordinates ( -1 : 1 ) (NDC)
float coordToNDC(float value, float fromMin, float fromMax);
//funnily enough for the width and height formula is a bit different
float dimensionsToNDC(float value, float fromMin, float fromMax);

void setupGLFWandGLAD(GLFW_Framework* framework);

void setupTextureParameters();

void setupShaders(GLFW_Framework* framework);

void setupGlobals(GLFW_Framework* framework);

int run(GLFW_Framework*);


