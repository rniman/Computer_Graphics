#pragma once
#include "cuboid.h"
#include <random>

class block
{
public:
	GLuint vao;
	GLuint vbo[2];

	glm::vec3 pos;

	std::vector<GLfloat> vertex;
	std::vector<GLfloat> color;
	glm::mat4 transformation;
	
	block()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_int_distribution<int> dis(60, 150);

		pos.x = static_cast<GLfloat>(dis(gen));
		pos.y = -200.0f + 15.0f;
		pos.z = static_cast<GLfloat>(dis(gen));

		makeCuboid(vertex, 50, 15);
		setCol(color, 0.3f, 0.3f, 0.3f);

		transformation = glm::mat4(1.0f);
	}

	GLvoid initBuffuer();
	GLvoid draw(unsigned int& modelLocation);
	GLvoid posModel();
};


GLvoid block::initBuffuer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

GLvoid block::draw(unsigned int& modelLocation)
{
	glFrontFace(GL_CCW);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);

	glFrontFace(GL_CW);
}

GLvoid block::posModel()
{
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, glm::vec3(pos.x, pos.y, pos.z));
}