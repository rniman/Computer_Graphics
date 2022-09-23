#pragma once
#include <iostream>
#include <random>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

const GLint window_w = 800;
const GLint window_h = 600;

class Rect
{
protected:
	GLfloat m_x = 0, m_y = 0;
	GLfloat m_width;
	GLfloat m_height;
	GLboolean m_state;
	GLfloat m_red, m_green, m_blue;
public:
	Rect()
	{
		m_state = false;
		m_red = 0;
		m_green = 0;
		m_blue = 0;
	}

	Rect(const GLfloat& i_x, const GLfloat& i_y)
		:m_x(i_x),m_y(i_y)
	{
		m_state = true;
	}

	GLfloat getX() const;
	GLfloat getY() const;
	GLboolean getState() const;
	GLfloat getWidth() const;
	GLfloat getHeight() const;
	GLfloat getRed() const;
	GLfloat getGreen() const;
	GLfloat getBlue() const;

	GLvoid setX(const GLfloat& i_x);
	GLvoid setY(const GLfloat& i_y);
	GLvoid setState(const GLboolean& i_state);
	GLvoid setRect(const int& i_x, const int& i_y);

	friend GLboolean collision(const Rect& r1, const  Rect& r2);
};

GLfloat Rect::getX() const
{
	return m_x;
}

GLfloat Rect::getY() const
{
	return m_y;
}

GLfloat Rect::getWidth() const
{
	return m_width;
}

GLfloat Rect::getHeight() const
{
	return m_height;
}

GLboolean Rect::getState() const
{
	return m_state;
}

GLfloat Rect::getRed() const
{
	return m_red;
}

GLfloat Rect::getGreen() const
{
	return m_green;
}

GLfloat Rect::getBlue() const
{
	return m_blue;
}

GLvoid Rect::setX(const GLfloat& i_x)
{
	m_x = i_x;
}

GLvoid Rect::setY(const GLfloat& i_y)
{
	m_y = i_y;
}

GLvoid Rect::setState(const GLboolean& i_state)
{
	m_state = i_state;
}

GLvoid Rect::setRect(const int& i_x, const int& i_y)
{
	m_x = (float)((i_x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0))) - m_width/2;
	m_y = -(float)((i_y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0))) - m_height/2;
	m_state = true;
}

GLboolean collision(const Rect& r1, const  Rect& r2)
{
	if ((r1.m_x < r2.m_x + r2.m_width) && (r1.m_y + r1.m_height > r2.m_y)
		&& (r1.m_x + r1.m_width > r2.m_x) && (r1.m_y < r2.m_y + r2.m_height))
	{
		return true;
	}
	else
		return false;
}

class my_Rect : public Rect
{
private:

public:
	my_Rect()
		:Rect()
	{
		m_width = 0.12f;
		m_height = 0.16f;
	}

	my_Rect(const GLfloat& i_x1, const GLfloat& i_y1)
		:Rect(i_x1, i_y1)
	{
		m_height = 0.12f;
		m_width = 0.16f;
	}
};



class obj_Rect : public Rect
{
private:

public:
	obj_Rect()
		:Rect()
	{
		m_width = 0.06f;
		m_height = 0.08f;
	}

	obj_Rect(const GLfloat& i_x1, const GLfloat& i_y1)
		:Rect(i_x1, i_y1)
	{
		m_width = 0.06f;
		m_height = 0.08f;
	}

	GLvoid setColor(const GLfloat& red, const GLfloat& green, const GLfloat& blue);
};

GLvoid obj_Rect::setColor(const GLfloat& red,const GLfloat& green, const GLfloat& blue)
{
	m_red = red;
	m_blue = blue;
	m_green = green;
}
