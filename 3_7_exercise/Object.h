#pragma once
#include "make_Shader.h"
#include <vector>


class object
{
public:
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> color;
	glm::mat4 transformation = glm::mat4(1.0f);

	GLint getSize() const;
		
};

GLint object::getSize() const
{
	return vertex.size() / 3;
}