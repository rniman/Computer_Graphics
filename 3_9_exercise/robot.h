#pragma once
#include "make_Shader.h"
#include <vector>

class robot
{
public:
	GLuint vao_body;
	GLuint vao_head;
	GLuint vao_arm;
	GLuint vao_leg;
	GLuint vao_nose;

	GLuint vbo_body[2];
	GLuint vbo_head[2];
	GLuint vbo_arm[2];
	GLuint vbo_leg[2];
	GLuint vbo_nose[2];

	std::vector<GLfloat> body;
	std::vector<GLfloat> head;
	std::vector<GLfloat> arm;
	std::vector<GLfloat> leg;
	std::vector<GLfloat> nose;

	std::vector<GLfloat> body_color;
	std::vector<GLfloat> head_color;
	std::vector<GLfloat> leg_color;
	std::vector<GLfloat> arm_color;
	std::vector<GLfloat> nose_color;

	glm::mat4 bodyTrans = glm::mat4(1.0f);
	glm::mat4 headTrans = glm::mat4(1.0f);
	glm::mat4 legTrans = glm::mat4(1.0f);
	glm::mat4 armTrans = glm::mat4(1.0f);
	glm::mat4 noseTrans = glm::mat4(1.0f);


	robot()
	{
		body =
		{
			//¾Õ¸é
				-30.0f, -30.0f, 30.0f,
				30.0f, 30.0f, 30.0f,
				-30.0f, 30.0f, 30.0f,

				-30.0f, -30.0f, 30.0f,
				30.0f, -30.0f, 30.0f,
				30.0f, 30.0f, 30.0f
			,
			//¿À¸¥¸é
				30.0f, -30.0f, 30.0f,
				30.0f, 30.0f, -30.0f,
				30.0f, 30.0f, 30.0f,

				30.0f, -30.0f, 30.0f,
				30.0f, -30.0f, -30.0f,
				30.0f, 30.0f, -30.0f
			,
			//µÞ¸é
				30.0f, -30.0f, -30.0f,
				-30.0f, -30.0f, -30.0f,
				-30.0f, 30.0f, -30.0f,

				30.0f, -30.0f, -30.0f,
				-30.0f, 30.0f, -30.0f,
				30.0f, 30.0f, -30.0f
			,
			//¿Þ¸é
				-30.0f, -30.0f, -30.0f,
				-30.0f, -30.0f, 30.0f,
				-30.0f, 30.0f, 30.0f,

				-30.0f, -30.0f, -30.0f,
				-30.0f, 30.0f, 30.0f,
				-30.0f, 30.0f, -30.0f
			,
			//À­¸é
				30.0f, 30.0f, 30.0f,
				30.0f, 30.0f, -30.0f,
				-30.0f, 30.0f, 30.0f,

				-30.0f, 30.0f, 30.0f,
				30.0f, 30.0f, -30.0f,
				-30.0f, 30.0f, -30.0f
			,
			//µÞ¸é
				30.0f, -30.0f, 30.0f,
				-30.0f, -30.0f, 30.0f,
				-30.0f, -30.0f, -30.0f,

				30.0f, -30.0f, 30.0f,
				-30.0f, -30.0f, -30.0f,
				30.0f, -30.0f, -30.0f
		};

		head =
		{
			//¾Õ¸é
				-15.0f, -15.0f, 15.0f,
				15.0f, 15.0f, 15.0f,
				-15.0f, 15.0f, 15.0f,

				-15.0f, -15.0f, 15.0f,
				15.0f, -15.0f, 15.0f,
				15.0f, 15.0f, 15.0f
			,
			//¿À¸¥¸é
				15.0f, -15.0f, 15.0f,
				15.0f, 15.0f, -15.0f,
				15.0f, 15.0f, 15.0f,

				15.0f, -15.0f, 15.0f,
				15.0f, -15.0f, -15.0f,
				15.0f, 15.0f, -15.0f
			,
			//µÞ¸é
				15.0f, -15.0f, -15.0f,
				-15.0f, -15.0f, -15.0f,
				-15.0f, 15.0f, -15.0f,

				15.0f, -15.0f, -15.0f,
				-15.0f, 15.0f, -15.0f,
				15.0f, 15.0f, -15.0f
			,
			//¿Þ¸é
				-15.0f, -15.0f, -15.0f,
				-15.0f, -15.0f, 15.0f,
				-15.0f, 15.0f, 15.0f,

				-15.0f, -15.0f, -15.0f,
				-15.0f, 15.0f, 15.0f,
				-15.0f, 15.0f, -15.0f
			,
			//À­¸é
				15.0f, 15.0f, 15.0f,
				15.0f, 15.0f, -15.0f,
				-15.0f, 15.0f, 15.0f,

				-15.0f, 15.0f, 15.0f,
				15.0f, 15.0f, -15.0f,
				-15.0f, 15.0f, -15.0f
			,
			//µÞ¸é
				15.0f, -15.0f, 15.0f,
				-15.0f, -15.0f, 15.0f,
				-15.0f, -15.0f, -15.0f,

				15.0f, -15.0f, 15.0f,
				-15.0f, -15.0f, -15.0f,
				15.0f, -15.0f, -15.0f
		};

		arm =
		{

		};

		leg =
		{

		};

		nose =
		{

		};

	}

	GLvoid initBuffer(GLuint& vao, GLuint vbo[], std::vector<GLfloat>& vertex, std::vector<GLfloat>& color);
	GLvoid init();
	GLvoid DrawRobot(GLuint& vao, GLuint vbo[], std::vector<GLfloat>& vertex, glm::mat4& transformation, unsigned int& modelLocation);
	GLvoid Draw(unsigned int& modelLocation);
};

GLvoid robot::initBuffer(GLuint& vao, GLuint vbo[], std::vector<GLfloat>& vertex, std::vector<GLfloat>& color)
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

GLvoid robot::init()
{
	initBuffer(vao_body, vbo_body, body, body_color);
	initBuffer(vao_leg, vbo_leg, leg, leg_color);
	initBuffer(vao_arm, vbo_arm, arm, arm_color);
	initBuffer(vao_nose, vbo_nose, nose, nose_color);
	initBuffer(vao_head, vbo_head, head, head_color);
}

GLvoid robot::DrawRobot(GLuint& vao, GLuint vbo[], std::vector<GLfloat>& vertex, glm::mat4& transformation, unsigned int& modelLocation)
{
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}

GLvoid robot::Draw(unsigned int& modelLocation)
{
	DrawRobot(vao_body, vbo_body, body, bodyTrans, modelLocation);
	DrawRobot(vao_head, vbo_head, body, bodyTrans, modelLocation);
	DrawRobot(vao_arm, vbo_arm, arm, armTrans, modelLocation);
	DrawRobot(vao_leg, vbo_leg, leg, legTrans, modelLocation);
	DrawRobot(vao_nose, vbo_nose, nose, noseTrans, modelLocation);
}