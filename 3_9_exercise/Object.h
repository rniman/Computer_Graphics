#pragma once
#include "make_Shader.h"
#include <vector>


class object
{
public:
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> color;
	std::vector<glm::mat4> transformation;

	GLint getSize() const;
		
};

GLint object::getSize() const
{
	return vertex.size() / 3;
}