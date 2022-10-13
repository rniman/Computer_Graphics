#pragma once
#include <vector>
#include "make_Shader.h"

struct axes_coordination
{
	glm::mat4 transformations = glm::mat4(1.0f);

	std::vector<GLfloat> axes_vertex =
	{
		//x��
		-2.0f,0.0f,0.0f,
		2.0f,0.0f,0.0f,
		//y��
		0.0f,-2.0f,0.0f,
		0.0f,2.0f,0.0f,
		//z��
		0.0f,0.0f,-2.0f,
		0.0f,0.0f,2.0f
	};

	std::vector<GLfloat> axes_color =
	{
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,

		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f
	};
};
