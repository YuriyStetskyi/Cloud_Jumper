#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define DEBUG

class Shader
{
public:

	// the program ID
	unsigned int ID;

	//default constructor just so you can make global shader
	Shader()
	{
		ID = 0;
	}

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vshaderFile;
		std::fstream fshaderfile;

		//make it so that ifstream throws exceptions
		vshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fshaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//open files
			vshaderFile.open(vertexPath);
			fshaderfile.open(fragmentPath);
			std::stringstream vShaderStream;
			std::stringstream fShaderStream;

			vShaderStream << vshaderFile.rdbuf();
			fShaderStream << fshaderfile.rdbuf();

			vshaderFile.close();
			fshaderfile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (std::ifstream::failure exception)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		unsigned int vertex = 0;
		unsigned int fragment = 0;


		//compile vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

#pragma region CHECK IF VERTEX SHADER COMPILED
		//set new success and infolog 
		int successV;
		char infoLogV[512];

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &successV);
		if (!successV)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLogV);
			std::cout << infoLogV << std::endl;
		}
		else
		{
#ifdef DEBUG
			std::cout << "SHADER::VERTEX::COMPILATION_SUCCESSFULL" << std::endl;
#endif // DEBUG

		}
#pragma endregion


		//compile fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

#pragma region CHECK IF FRAGMENT SHADER COMPILED
		//set new success and infolog 
		int successF;
		char infoLogF[512];

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &successF);
		if (!successF)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLogF);
			std::cout << infoLogF << std::endl;
		}
		else
		{
#ifdef DEBUG
			std::cout << "SHADER::FRAGMENT::COMPILATION_SUCCESSFULL" << std::endl;
#endif // DEBUG
		}
#pragma endregion

		//link program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

#pragma region CHECK IF PROGRAM LINKED
		//set new success and infolog
		int successP;
		char infoLogP[512];

		glGetProgramiv(ID, GL_LINK_STATUS, &successP);
		if (!successP)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLogP);
			std::cout << infoLogP << std::endl;
		}
		else
		{
#ifdef DEBUG
			std::cout << "SHADER::PROGRAM::LINKAGE_SUCCESSFULL" << std::endl;
#endif // DEBUG

		}
#pragma endregion

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	// use/activate the shader
	void use()
	{
		glUseProgram(ID);
	}

	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

};

#endif // SHADER_H

