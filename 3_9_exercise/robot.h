#pragma once
#include "cuboid.h"
#include "block.h"

GLvoid makeHexa(std::vector<GLfloat>& obj, const GLfloat& width, const GLfloat& hegiht);
GLvoid setCol(std::vector<GLfloat>& col, const GLfloat& red, const GLfloat& green, const GLfloat& blue);

class robot
{
public:
	glm::vec3 pos;

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
	glm::mat4 leftLegTrans = glm::mat4(1.0f);
	glm::mat4 rightLegTrans = glm::mat4(1.0f);
	glm::mat4 leftArmTrans = glm::mat4(1.0f);
	glm::mat4 rightArmTrans = glm::mat4(1.0f);
	glm::mat4 noseTrans = glm::mat4(1.0f);

	GLint Xdir, Zdir;
	GLfloat look;
	
	GLboolean swing;
	GLfloat swingLeg;
	GLfloat swingArm;

	GLboolean jumpState;
	GLboolean fallDown;
	GLfloat jumpHeight;
	robot()
	{
		pos = glm::vec3(0.0f, -130.0f, 0.0f);

		makeCuboid(body, 30.0f, 30.0f);
		makeCuboid(head, 15.0f, 15.0f);
		makeCuboid(arm, 10.0f, 20.0f);
		makeCuboid(leg, 10.0f, 40.0f);
		makeCuboid(nose, 5.0f, 5.0f);
		
		setCol(body_color, 0.0f, 0.0f, 0.0f);
		setCol(head_color, 0.7f, 0.7f, 0.7f);
		setCol(arm_color, 0.0f, 0.5f, 0.5f);
		setCol(leg_color, 1.0f, 0.5f, 0.0f);
		setCol(nose_color, 0.0f, 1.0f, 0.0f);

		Xdir = 0;
		Zdir = 0;
		look = 90.0f;

		swing = true;
		swingArm = 0.0f;
		swingLeg = 0.0f;

		jumpState = false;
		fallDown = false;
		jumpHeight = 0.0f;
	}

	GLvoid init();
	GLvoid Draw(unsigned int& modelLocation);
	GLvoid initMatrix();
	GLvoid initRobotModel();
	GLvoid posModel(const glm::vec3& vector);
	GLvoid lookModel();
	GLvoid swingModel();
	GLvoid changeLook();
	GLvoid move(const block& bl);
	GLvoid swingRobot();
	GLvoid telepotePos();
	GLvoid jumpRobot(const block& bl);
	GLvoid jump();
	
	//friend GLboolean checkFloor(const robot& rb, const block);
	friend GLboolean outStage(const robot& rb);
	friend GLboolean checkBlock(const block& bl, const robot& rb);

};



GLvoid initBuffer(GLuint& vao, GLuint (&vbo)[2], std::vector<GLfloat>& vertex, std::vector<GLfloat>& color)
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

GLvoid DrawRobot(GLuint& vao, GLuint (&vbo)[2], std::vector<GLfloat>& vertex, glm::mat4& transformation, unsigned int& modelLocation)
{
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);
}

GLvoid robot::init()
{
	initBuffer(vao_body, vbo_body, body, body_color);
	initBuffer(vao_leg, vbo_leg, leg, leg_color);
	initBuffer(vao_arm, vbo_arm, arm, arm_color);
	initBuffer(vao_nose, vbo_nose, nose, nose_color);
	initBuffer(vao_head, vbo_head, head, head_color);
}

GLvoid robot::Draw(unsigned int& modelLocation)
{
	glFrontFace(GL_CCW);
	DrawRobot(vao_body, vbo_body, body, bodyTrans, modelLocation);
	DrawRobot(vao_head, vbo_head, head, headTrans, modelLocation);
	DrawRobot(vao_arm, vbo_arm, arm, leftArmTrans, modelLocation);
	DrawRobot(vao_arm, vbo_arm, arm, rightArmTrans, modelLocation);
	DrawRobot(vao_leg, vbo_leg, leg, leftLegTrans, modelLocation);
	DrawRobot(vao_leg, vbo_leg, leg, rightLegTrans, modelLocation);
	DrawRobot(vao_nose, vbo_nose, nose, noseTrans, modelLocation);
	glFrontFace(GL_CW);

}

GLvoid robot::initMatrix()
{
	bodyTrans = glm::mat4(1.0f);
	headTrans = glm::mat4(1.0f);
	leftLegTrans = glm::mat4(1.0f);
	rightLegTrans = glm::mat4(1.0f);
	leftArmTrans = glm::mat4(1.0f);
	rightArmTrans = glm::mat4(1.0f);
	noseTrans = glm::mat4(1.0f);
}

GLvoid robot::initRobotModel()
{
	headTrans = glm::translate(headTrans, glm::vec3(0.0f, 40.0f, 0.0f));
	rightLegTrans = glm::translate(rightLegTrans, glm::vec3(15.0f, -30.0f, 0.0f));
	leftLegTrans = glm::translate(leftLegTrans, glm::vec3(-15.0f, -30.0f, 0.0f));
	leftArmTrans = glm::translate(leftArmTrans, glm::vec3(-35.0f, 0.0f, 0.0f));
	rightArmTrans = glm::translate(rightArmTrans, glm::vec3(35.0f, 0.0f, 0.0f));
	noseTrans = glm::translate(noseTrans, glm::vec3(0.0f, 40.0f, 15.0f));
}

GLvoid robot::posModel(const glm::vec3& vector)
{
	bodyTrans = glm::translate(bodyTrans, vector);
	headTrans = glm::translate(headTrans, vector);
	rightLegTrans = glm::translate(rightLegTrans, vector);
	leftLegTrans = glm::translate(leftLegTrans, vector);
	leftArmTrans = glm::translate(leftArmTrans, vector);
	rightArmTrans = glm::translate(rightArmTrans, vector);
	noseTrans = glm::translate(noseTrans, vector);
}

GLvoid robot::lookModel()
{
	bodyTrans = glm::rotate(bodyTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
	headTrans = glm::rotate(headTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
	rightLegTrans = glm::rotate(rightLegTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
	leftLegTrans = glm::rotate(leftLegTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
	leftArmTrans = glm::rotate(leftArmTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
	rightArmTrans = glm::rotate(rightArmTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
	noseTrans = glm::rotate(noseTrans, glm::radians(look), glm::vec3(0.0f, 1.0f, 0.0f));
}

GLvoid robot::swingModel()
{
	rightLegTrans = glm::rotate(rightLegTrans, glm::radians(swingLeg), glm::vec3(1.0f, 0.0f, 0.0f));
	leftLegTrans = glm::rotate(leftLegTrans, glm::radians(-swingLeg), glm::vec3(1.0f, 0.0f, 0.0f));
	leftArmTrans = glm::rotate(leftArmTrans, glm::radians(swingArm), glm::vec3(1.0f, 0.0f, 0.0f));
	rightArmTrans = glm::rotate(rightArmTrans, glm::radians(-swingArm), glm::vec3(1.0f, 0.0f, 0.0f));
}

GLvoid robot::changeLook()
{
	if (Xdir == 1 && Zdir == 0)
	{
		look = 90.0f;
	}
	else if (Xdir == -1 && Zdir == 0)
	{
		look = -90.0f;
	}
	else if (Xdir == 0 && Zdir == 1)
	{
		look = 0.0f;
	}
	else if (Xdir == 0 && Zdir == -1)
	{
		look = 180.0f;
	}
	else if (Xdir == 1 && Zdir == 1)
	{
		look = 45.0f;
	}
	else if (Xdir == -1 && Zdir == 1)
	{
		look = -45.0f;
	}
	else if (Xdir == -1 && Zdir == -1)
	{
		look = -135.0f;
	}
	else if (Xdir == 1 && Zdir == -1)
	{
		look = 135.0f;
	}
}


GLvoid robot::move(const block& bl)
{
	pos.x += Xdir * 10.0f;
	pos.z += Zdir * 10.0f;

	if (Xdir != 0 || Zdir != 0)
	{
		swingRobot();
	}

	if (outStage(*this))
	{
		telepotePos();
	}

	//장애물 체크
	if (checkBlock(bl, *this))
	{
		pos.x -= Xdir * 10.0f;
		pos.z -= Zdir * 10.0f;
	}

	if (pos.y > -130.0f && !jumpState)
	{
		if ((pos.x <= bl.pos.x - 50.0f || pos.x >= bl.pos.x + 50.0f) ||
			(pos.z <= bl.pos.z - 50.0f || pos.z >= bl.pos.z + 50.0f))
		{
			pos.y -= 10.0f;
		}
	}
}

GLvoid robot::swingRobot()
{
	if (swing)
	{
		swingArm += 7.5f;
		swingLeg += 5.0;

		if (swingArm == 45.0f)
			swing = false;
	}
	else
	{
		swingArm -= 7.5f;
		swingLeg -= 5.0;

		if (swingArm == -45.0f)
			swing = true;
	}
}

GLvoid robot::telepotePos()
{
	if (pos.x <= -200.f)
		pos.x = 190.0f;
	else if (pos.x >= 200.0f)
		pos.x = -190.0f;

	if (pos.z <= -200.f)
		pos.z = 190.0f;
	else if (pos.z >= 200.0f)
		pos.z = -190.0f;
}


GLvoid robot::jumpRobot(const block& bl)
{

	if (!fallDown)
	{
		jumpHeight += 10.0f;
		pos.y += 10.0f;
		if (jumpHeight == 100.0f)
			fallDown = true;
	}
	else
	{
		jumpHeight -= 10.0f;
		pos.y -= 10.0f;
		if (checkBlock(bl, *this) || pos.y == -130.0f)
		{
			fallDown = false;
			jumpState = false;
			jumpHeight = 0.0f;
			pos.y += 10.0f;
		}
	}
}



GLboolean outStage(const robot& rb)
{
	if (rb.pos.x >= 200.0f || rb.pos.x <= -200.0f || rb.pos.z >= 200.0f || rb.pos.z <= -200.0f)
	{
		return true;
	}

	return false;
}

GLboolean checkBlock(const block& bl, const robot& rb)
{
	if (rb.pos.x >= bl.pos.x - 50.0f && rb.pos.x <= bl.pos.x + 50.0f &&
		rb.pos.z >= bl.pos.z - 50.0f && rb.pos.z <= bl.pos.z + 50.0f)
	{
		if (rb.pos.y - 70 < bl.pos.y + 15.0f)
		{
			return true;
		}
	}
	return false;
}