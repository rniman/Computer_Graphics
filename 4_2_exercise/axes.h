#pragma once
#include "make_Shader.h"

struct axes_coordination
{
	glm::mat4 transformation = glm::mat4(1.0f);

	std::vector<GLfloat> axes_vertex =
	{
		//x��
		-1000.0f,0.0f,0.0f,
		1000.0f,0.0f,0.0f,
		//y��
		0.0f,-1000.0f,0.0f,
		0.0f,1000.0f,0.0f,
		//z��
		0.0f,0.0f,-1000.0f,
		0.0f,0.0f,1000.0f
	};

	std::vector<GLfloat> axes_color =
	{
		0.3f,0.7f,0.0f,
		0.3f,0.7f,0.0f,
		
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,

		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f
	};
};
