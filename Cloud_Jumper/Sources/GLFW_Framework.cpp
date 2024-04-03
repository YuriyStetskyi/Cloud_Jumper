#include "GLFW_Framework.h"

int GLOBAL_SCREEN_WIDTH;
int GLOBAL_SCREEN_HEIGHT;
int GLOBAL_FULLSCREEN;

Sprite::Sprite()
{
	textureObject = 0;
	width = 0;
	height = 0;
	numberOfChannels = 0;
	textureData = nullptr;
	X = 0.0f;
	Y = 0.0f;
	ndcX = 0.0f;
	ndcY = 0.0f;
	ndcWidth = 0.0f;
	ndcHeight = 0.0f;
	VAO = -1;
	VBO = -1;
	EBO = -1;

}

Sprite::~Sprite()
{
	stbi_image_free(textureData);
}

Sprite* createSprite(const char* path)
{
	//init sprites
	//DONT FLIP VERTICALLY ON LOAD BECAUSE THIS PROJECT WAS MIGRATED FROM
	//ANOTHER ENGINE THAT HAD DIFFERENT RENDERING COORDINATE SYSTEM.
	//stbi_set_flip_vertically_on_load(true);
	Sprite* sprite = new Sprite();

	

	glGenTextures(1, &(sprite->textureObject));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->textureObject);

	sprite->textureData = stbi_load(path, &(sprite->width), &(sprite->height), &(sprite->numberOfChannels), 0);
		
	if (sprite->textureData)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->width, sprite->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite->textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "TEXTURE::LOAD_SUCCESSFULL" << std::endl;
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOAD_FAILURE" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);


	//init the buffers and set up attributes positions and strides
	unsigned int indices[] =
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &sprite->VAO);
	glGenBuffers(1, &sprite->VBO);
	glGenBuffers(1, &sprite->EBO);

	glBindVertexArray(sprite->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinates attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return sprite;
}

void drawSprite(Sprite* sprite, int x, int y)
{	
	//vertex order : top-right, bottom-right, bottom-left, top-left
	sprite->X = x;
	sprite->Y = y;
	sprite->ndcX = coordToNDC(x, 0, GLOBAL_SCREEN_WIDTH);
	sprite->ndcY = coordToNDC(y, 0, GLOBAL_SCREEN_HEIGHT);
	sprite->ndcWidth = dimensionsToNDC(sprite->width, 0, GLOBAL_SCREEN_WIDTH);
	sprite->ndcHeight = dimensionsToNDC(sprite->height, 0, GLOBAL_SCREEN_HEIGHT);

	float vertices[32];
	calculateSpriteVertices(sprite, vertices, 32);
	
	glBindVertexArray(sprite->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->textureObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void getSpriteSize(Sprite* s, int& w, int& h)
{
	w = s->width;
	h = s->height;
}

void setSpriteSize(Sprite* s, int w, int h)
{
	s->width = w;
	s->height = h;
	s->ndcWidth = dimensionsToNDC(s->width, 0, GLOBAL_SCREEN_WIDTH);
	s->ndcHeight = dimensionsToNDC(s->height, 0, GLOBAL_SCREEN_HEIGHT);
}

void destroySprite(Sprite* s)
{
	delete s;
}

void drawTestBackground()
{
}

void getScreenSize(int& w, int& h)
{
	w = GLOBAL_SCREEN_WIDTH;
	h = GLOBAL_SCREEN_HEIGHT;
}

unsigned int getTickCount()
{
	return 0;
}

void showCursor(bool bShow)
{
}

void GLFW_Framework::processInput(GLFWwindow* window)
{
	//mouse movement
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	onMouseMove(xPos, yPos, xPos, yPos);


	//key presses

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (keys_pressed[FRKey::RIGHT] == false)
		{
			keys_pressed[FRKey::RIGHT] = true;
			onKeyPressed(FRKey::RIGHT);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{	
		if (keys_pressed[FRKey::LEFT] == false)
		{
			keys_pressed[FRKey::LEFT] = true;
			onKeyPressed(FRKey::LEFT);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (keys_pressed[FRKey::DOWN] == false)
		{
			keys_pressed[FRKey::DOWN] = true;
			onKeyPressed(FRKey::DOWN);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (keys_pressed[FRKey::UP] == false)
		{
			keys_pressed[FRKey::UP] = true;
			onKeyPressed(FRKey::UP);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (keys_pressed[FRKey::A] == false)
		{
			keys_pressed[FRKey::A] = true;
			onKeyPressed(FRKey::A);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (keys_pressed[FRKey::D] == false)
		{
			keys_pressed[FRKey::D] = true;
			onKeyPressed(FRKey::D);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		if (keys_pressed[FRKey::ESCAPE] == false)
		{
			keys_pressed[FRKey::ESCAPE] = true;
			onKeyPressed(FRKey::ESCAPE);
		}
	}

	//key releases

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
	{

		if (keys_pressed[FRKey::RIGHT] == true)
		{
			keys_pressed[FRKey::RIGHT] = false;
			onKeyReleased(FRKey::RIGHT);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
	{
		if (keys_pressed[FRKey::LEFT] == true)
		{
			keys_pressed[FRKey::LEFT] = false;
			onKeyReleased(FRKey::LEFT);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
	{
		if (keys_pressed[FRKey::DOWN] == true)
		{
			keys_pressed[FRKey::DOWN] = false;
			onKeyReleased(FRKey::DOWN);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
	{
		if (keys_pressed[FRKey::UP] == true)
		{
			keys_pressed[FRKey::UP] = false;
			onKeyReleased(FRKey::UP);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
	{
		if (keys_pressed[FRKey::A] == true)
		{
			keys_pressed[FRKey::A] = false;
			onKeyReleased(FRKey::A);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
	{
		if (keys_pressed[FRKey::D] == true)
		{
			keys_pressed[FRKey::D] = false;
			onKeyReleased(FRKey::D);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		if (keys_pressed[FRKey::ESCAPE] == true)
		{
			keys_pressed[FRKey::ESCAPE] = false;
			onKeyReleased(FRKey::ESCAPE);
		}
	}

	//mouse presses

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (mouseButtons_pressed[FRMouseButton::LEFT] == false)
		{
			mouseButtons_pressed[FRMouseButton::LEFT] = true;
			onMouseButtonClick(FRMouseButton::LEFT, false);
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (mouseButtons_pressed[FRMouseButton::MIDDLE] == false)
		{
			mouseButtons_pressed[FRMouseButton::MIDDLE] = true;
			onMouseButtonClick(FRMouseButton::MIDDLE, false);
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (mouseButtons_pressed[FRMouseButton::RIGHT] == false)
		{
			mouseButtons_pressed[FRMouseButton::RIGHT] = true;
			onMouseButtonClick(FRMouseButton::RIGHT, false);
		}
	}

	//mouse releases

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		if (mouseButtons_pressed[FRMouseButton::LEFT] == true)
		{
			mouseButtons_pressed[FRMouseButton::LEFT] = false;
			onMouseButtonClick(FRMouseButton::LEFT, true);
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		if (mouseButtons_pressed[FRMouseButton::MIDDLE] == true)
		{
			mouseButtons_pressed[FRMouseButton::MIDDLE] = false;
			onMouseButtonClick(FRMouseButton::MIDDLE, true);
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		if (mouseButtons_pressed[FRMouseButton::RIGHT] == true)
		{
			mouseButtons_pressed[FRMouseButton::RIGHT] = false;
			onMouseButtonClick(FRMouseButton::RIGHT, true);
		}
	}
}

void GLFW_Framework::setTitle()
{
	title = GetTitle();
}

void GLFW_Framework::runPreInit(int& width, int& height, bool& fullscreen)
{
	PreInit(width, height, fullscreen);
}

bool GLFW_Framework::runInit(GLFW_Framework* framework)
{
	title = "default title";

	for (int i = 0; i < (int)FRKey::COUNT; ++i)
	{
		keys_pressed.emplace((FRKey)i, false);
	}

	for (int i = 0; i < (int)FRMouseButton::COUNT; ++i)
	{
		mouseButtons_pressed.emplace((FRMouseButton)i, false);
	}

	framework->setTitle();
	setupGLFWandGLAD(framework);
	if (!framework->window)
	{
		return false;
	}

	return Init();
}

void GLFW_Framework::runClose()
{
	glfwSetWindowShouldClose(window, true);
	Close();
}

bool GLFW_Framework::runTick()
{
	return Tick();
}

void GLFW_Framework::PreInit(int& width, int& height, bool& fullscreen)
{
	
}

bool GLFW_Framework::Init()
{
	return true;
}

void GLFW_Framework::Close()
{
	
}

bool GLFW_Framework::Tick()
{
	return false;
}

void GLFW_Framework::onMouseMove(int x, int y, int xrelative, int yrelative)
{

}

void GLFW_Framework::onMouseButtonClick(FRMouseButton button, bool isReleased)
{


}

void GLFW_Framework::onKeyPressed(FRKey k)
{

}

void GLFW_Framework::onKeyReleased(FRKey k)
{

}

const char* GLFW_Framework::GetTitle()
{
	return "default window";
}

void calculateSpriteVertices(Sprite* sprite, float* vertices, int arrSize)
{
	//top-right
	vertices[0] = sprite->ndcWidth + sprite->ndcX;
	vertices[1] = sprite->ndcHeight + sprite->ndcY;
	vertices[2] = 0;
	vertices[3] = 1;
	vertices[4] = 1;
	vertices[5] = 1;
	vertices[6] = 1.0f;
	vertices[7] = 1.0f;

	//bottom-right
	vertices[8] = sprite->ndcWidth + sprite->ndcX;
	vertices[9] = sprite->ndcY;
	vertices[10] = 0;
	vertices[11] = 1;
	vertices[12] = 1;
	vertices[13] = 1;
	vertices[14] = 1.0f;
	vertices[15] = 0.0f;

	//bottom-left
	vertices[16] = sprite->ndcX;
	vertices[17] = sprite->ndcY;
	vertices[18] = 0;
	vertices[19] = 1;
	vertices[20] = 1;
	vertices[21] = 1;
	vertices[22] = 0.0f;
	vertices[23] = 0.0f;

	//top-left
	vertices[24] = sprite->ndcX;
	vertices[25] = sprite->ndcHeight + sprite->ndcY;
	vertices[26] = 0;
	vertices[27] = 1;
	vertices[28] = 1;
	vertices[29] = 1;
	vertices[30] = 0.0f;
	vertices[31] = 1.0f;
}

float coordToNDC(float value, float fromMin, float fromMax)
{
	return -1 + (1 - (-1)) * ((value - fromMin) / (fromMax - fromMin));
}

float dimensionsToNDC(float value, float fromMin, float fromMax)
{
	//float dimensionLength = 0 + (2 - 0) * ((value - fromMin) / (fromMax - fromMin));
	return 0 + (2 - 0) * ((value - fromMin) / (fromMax - fromMin));
}

void setupGLFWandGLAD(GLFW_Framework* framework)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	framework->window = glfwCreateWindow(framework->screen_width, framework->screen_height, framework->title.c_str(), NULL, NULL);
	glfwSetWindowShouldClose(framework->window, 0);
	if (!framework->window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(framework->window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, framework->screen_width, framework->screen_height);
}

void setupTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//setup opacity
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void setupShaders(GLFW_Framework* framework)
{
	framework->shader = Shader(framework->vertexShader_path.c_str(), framework->fragmentShader_path.c_str());
	framework->shader.use();
}

void setupGlobals(GLFW_Framework* framework)
{
	GLOBAL_SCREEN_WIDTH = framework->screen_width;
	GLOBAL_SCREEN_HEIGHT = framework->screen_height;
}


int run(GLFW_Framework* framework)
{
	framework->runPreInit(framework->screen_width, framework->screen_height, framework->screen_fullscreen);
	setupGlobals(framework);
	if (framework->runInit(framework))
	{
		

		setupTextureParameters();
		setupShaders(framework);

		while (!glfwWindowShouldClose(framework->window))
		{
			glClearColor(0.25f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			framework->processInput(framework->window);

			if (framework->runTick())
			{
				framework->runClose();
			}

			glfwSwapBuffers(framework->window);
			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
}