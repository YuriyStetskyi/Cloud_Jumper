#pragma once

//need this define for sprintf function to work that is inside stb_image_write.h
#define _CRT_SECURE_NO_WARNINGS
//need these defines for stb_image to work. stb_image is used in GLFW_Framework
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

//#define DEBUG

//#include "GLFW_Framework.h"
#include "Game_Framework.h"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>


Game_Framework* glfw_framework;
int width = 1200;
int height = 800;
bool restartClient = true;
bool fullscreen = false;

void CheckForResolutionChange(Game_Framework* framework)
{
    while (true)
    {
        std::string consoleInput;
        std::getline(std::cin, consoleInput);

        if (consoleInput != "")
        {
            std::istringstream iss(consoleInput);
            std::string firstWord;
            std::string secondWord;
            std::string resolution;

            iss >> firstWord >> secondWord >> resolution;
            if (firstWord == "game" && secondWord == "-window")
            {
                char x;
                int w_input, h_input;
                std::istringstream resolutionStream(resolution);
                if (!(resolutionStream >> w_input >> x >> h_input) || x != 'x')
                {
                    std::cout << "Invalid resolution format.\n" <<
                        "correct format: WidthxHeight\n";
                }
                else
                {
                    width = w_input;
                    height = h_input;
                    restartClient = true;
                    glfw_framework->Close();
                    std::cout << "resolution changed successfully!" << std::endl;
                }
            }
            else
            {
                std::cout << "Invalid command.\n" <<
                    "To change resolution use command:\n" <<
                    "game -window WidthxHeight\n";
            }
        }
    }
}
std::thread console_thread(CheckForResolutionChange, glfw_framework);


int main()
{
	console_thread.detach();
	std::cout << "To change resolution use command:\ngame -window 'width'x'height'" << std::endl;
    
    /*width = 1200;
    height = 600;*/
    

    while (restartClient)
    {
        glfw_framework = new Game_Framework;
        restartClient = false;
        glfw_framework->ChangePreInitStats(width, height, false);
        run(glfw_framework);
    }

	
	return 0;
}