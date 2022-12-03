#pragma once
#include "make_Shader.h"

GLuint shaderID_02;		//���̴� ���α׷� �̸�
GLuint vertexShader_not_texture;	//���ؽ� ���̴� ��ü
GLuint fragmentShader_not_texture;	//�����׸�Ʈ ���̴� ��ü

GLboolean make_vertexShader_not_texture()
{
	GLchar* vertexSource;

	//���ؽ� ���̴� �ڵ带 �о�ͼ� �����ϰ� �������� �ؾ���
	//filetobuf:��������� �Լ��� �ؽ�Ʈ�� �а� ���ڿ��� �����ϴ� �Լ�

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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return false;
	}
}
GLint make_shaderProgram_not_texture()
{
	make_vertexShader_not_texture();	//���ؽ� ���̴� �����
	make_fragementShader_not_texture();	//�����׸�Ʈ ���̴� �����

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //���̴� ���α׷� �����

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
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}

	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}

