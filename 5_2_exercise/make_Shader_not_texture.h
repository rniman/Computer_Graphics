#pragma once
#include "make_Shader.h"

GLuint shaderID_02;		//세이더 프로그램 이름
GLuint vertexShader_not_texture;	//버텍스 세이더 객체
GLuint fragmentShader_not_texture;	//프래그먼트 세이더 객체

GLboolean make_vertexShader_not_texture()
{
	GLchar* vertexSource;

	//버텍스 세이더 코드를 읽어와서 저장하고 컴파일을 해야함
	//filetobuf:사용자정의 함수로 텍스트를 읽고 문자열에 저장하는 함수

	vertexSource = filetobuf("vertex_not_texture.glsl");
	vertexShader_not_texture = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_not_texture, 1, &vertexSource, NULL);
	glCompileShader(vertexShader_not_texture);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader_not_texture, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader_not_texture, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return false;
	}
}

GLboolean make_fragementShader_not_texture()
{
	GLchar* fragmentSource;

	fragmentSource = filetobuf("fragment_not_texture.glsl");
	fragmentShader_not_texture = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_not_texture, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader_not_texture);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader_not_texture, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader_not_texture, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return false;
	}
}
GLint make_shaderProgram_not_texture()
{
	make_vertexShader_not_texture();	//버텍스 세이더 만들기
	make_fragementShader_not_texture();	//프래그먼트 세이더 만들기

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //세이더 프로그램 만들기

	glAttachShader(ShaderProgramID, vertexShader_not_texture);
	glAttachShader(ShaderProgramID, fragmentShader_not_texture);
	glLinkProgram(ShaderProgramID);

	glDeleteShader(vertexShader_not_texture);
	glDeleteShader(fragmentShader_not_texture);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}

	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}

