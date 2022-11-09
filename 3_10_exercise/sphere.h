#pragma once
#include "read_Obj.h"

class sphere
{
public:

	objRead object;
	GLuint vao;
	GLuint vbo_vertex;
	GLuint vbo_color;
	GLuint speed;

	std::vector<GLfloat> color;
	std::vector<glm::mat4> transformation;
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> dir;

	sphere()
	{}

	sphere(const int& i_num)
	{
		object.loadObj_normalize_center("sphere.obj");

		for (int i = 0; i < i_num; ++i)
		{
			std::uniform_int_distribution<int> dis(-10, 10);
			glm::vec3 temp(GLfloat(dis(gen)) * 0.1f, GLfloat(dis(gen)) * 0.1f, GLfloat(dis(gen)) * 0.1f);

			if (temp == glm::vec3(0.0f, 0.0f, 0.0f))
				temp.x += 1.0f;

			transformation.push_back(glm::mat4(1.0f));
			pos.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			dir.push_back(glm::normalize(temp));
		}
		setColor(object, color, 0.0f, 0.7f, 1.0f);
		speed = 10.0f;
	}

	GLvoid init();
	GLvoid draw(unsigned int& modelLocation);
	GLvoid setDir();
	GLvoid move();
	GLvoid model();
	GLvoid clamp(const GLfloat& lenth);

	GLvoid addSphere();
};

GLvoid sphere::init()
{
	initBuffer(vao, vbo_vertex, vbo_color, object.outvertex, color);
}

GLvoid sphere::draw(unsigned int& modelLocation)
{
	for (int i = 0; i < transformation.size(); ++i)
		drawObject(modelLocation, transformation[i], vao, object.outvertex.size());
}

GLvoid setDir()
{

}

GLvoid sphere::move()
{
	for (int i = 0; i < pos.size(); ++i)
	{
		pos[i].x += speed * dir[i].x;
		pos[i].y += speed * dir[i].y;
		pos[i].z += speed * dir[i].z;
	}
}

GLvoid sphere::model()
{
	for (int i = 0; i < transformation.size(); ++i)
	{
		transformation[i] = glm::mat4(1.0f);

		transformation[i] = glm::translate(transformation[i], pos[i]);
	}
}

GLvoid sphere::clamp(const GLfloat& lenth)
{

	for (int i = 0; i < pos.size(); ++i)
	{
		if (lenth * 0.5f <= pos[i].x + 50.0f)
		{
			dir[i] = glm::reflect(dir[i], glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if (-lenth * 0.5f >= pos[i].x - 50.0f)
		{
			dir[i] = glm::reflect(dir[i], glm::vec3(1.0f, 0.0f, 0.0f));
		}
		
		if (lenth * 0.5f <= pos[i].y + 50.0f)
		{
			dir[i] = glm::reflect(dir[i], glm::vec3(0.0f, -1.0f, 0.0f));
		}
		else if (-lenth * 0.5f >= pos[i].y - 50.0f)
		{
			dir[i] = glm::reflect(dir[i], glm::vec3(0.0f, 1.0f, 0.0f));
		}
		
		if (lenth * 0.5f <= pos[i].z + 50.0f)
		{
			dir[i] = glm::reflect(dir[i], glm::vec3(0.0f, 0.0f, -1.0f));
		}
		else if (-lenth * 0.5f >= pos[i].z - 50.0f)
		{
			dir[i] = glm::reflect(dir[i], glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
}

GLvoid sphere::addSphere()
{
	std::uniform_int_distribution<int> dis(-10, 10);
	glm::vec3 temp(GLfloat(dis(gen)) * 0.1f, GLfloat(dis(gen)) * 0.1f, GLfloat(dis(gen)) * 0.1f);

	if (temp == glm::vec3(0.0f, 0.0f, 0.0f))
		temp.x += 1.0f;

	transformation.push_back(glm::mat4(1.0f));
	pos.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	dir.push_back(glm::normalize(temp));
}