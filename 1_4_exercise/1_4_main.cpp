#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

const GLint window_w = 800;
const GLint window_h = 600;
const GLfloat red = 0.2f;
const GLfloat green = 0.2f;
const GLfloat blue = 0.2f;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid MouseEvent(int button, int state, int x, int y);
GLvoid TimerEvent(int value);

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 100);

GLboolean timer_on = false;

class rect
{
private:
	GLfloat left = 0, right = 0;
	GLfloat top = 0, bottom = 0;

	GLfloat ori_left = 0, ori_right = 0;
	GLfloat ori_top = 0, ori_bottom = 0;

	GLfloat red;
	GLfloat blue;
	GLfloat green;

	GLboolean move = false;
	GLint direct = 0;
public:
	rect()
	{
		red = (static_cast<float>(dis(gen)) * 0.01);
		green = (static_cast<float>(dis(gen)) * 0.01);
		blue = (static_cast<float>(dis(gen)) * 0.01);
	}

	rect(const int& x, const int& y)
	{
		red = (static_cast<float>(dis(gen)) * 0.01);
		green = (static_cast<float>(dis(gen)) * 0.01);
		blue = (static_cast<float>(dis(gen)) * 0.01);

		//�⺻ ũ�� 100
		convert_WindowXY_OpenglXY(x - 50, y + 50, left, bottom);
		convert_WindowXY_OpenglXY(x + 50, y - 50, right, top);
		ori_left = left;
		ori_right = right;
		ori_top = top;
		ori_bottom = bottom;
		std::uniform_int_distribution<int> dir_dis(0, 3);
		this->direct = dir_dis(gen);
	}

	GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
	GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

	GLfloat getLeft() const
	{
		return left;
	}

	GLfloat getRight() const
	{
		return right;
	}

	GLfloat getTop() const
	{
		return top;
	}

	GLfloat getBottom() const
	{
		return bottom;
	}

	GLfloat getRed() const
	{
		return red;
	}

	GLfloat getGreen() const
	{
		return green;
	}

	GLfloat getBlue() const
	{
		return blue;
	}

	GLint getDirect() const
	{
		return direct;
	}

	GLvoid setLeft(const GLfloat x)
	{
		left = x;
	}
	GLvoid setRight(const GLfloat x)
	{
		right = x;
	}
	GLvoid setTop(const GLfloat y)
	{
		top = y;
	}
	GLvoid setBottom(const GLfloat y)
	{
		bottom = y;
	}
	GLvoid setDirect(const GLint dir)
	{
		direct = dir;
	}

	GLvoid moveLeft(const GLfloat x)
	{
		left += x;
	}
	GLvoid moveRight(const GLfloat x)
	{
		right += x;
	}
	GLvoid moveTop(const GLfloat y)
	{
		top += y;
	}
	GLvoid moveBottom(const GLfloat y)
	{
		bottom += y;
	}

	GLvoid posReset()
	{
		left = ori_left;
		right = ori_right;
		top = ori_top;
		bottom = ori_bottom;
	}

	GLfloat getWidth() const
	{
		return right - left;
	}

	GLfloat getHeight() const
	{
		return top - bottom;
	}

};

GLvoid rect::convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy)
{
	x = static_cast<int>((window_w / 2) + ox * (window_w / 2));
	y = static_cast<int>((window_h / 2) - oy * (window_h / 2));
}

GLvoid rect::convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy)
{
	ox = (float)((x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0)));
	oy = -(float)((y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0)));
}

std::vector<rect> vec_rect;
GLint num_rect = 0;

//0 - s , 1 - a , 2 - i, 3 - c
GLint animation_num = 0;
GLboolean timer_a = false;
GLboolean timer_i = false;
GLboolean timer_c = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 50); // �������� ��ġ ����
	glutInitWindowSize(window_w, window_h); // �������� ũ�� ����
	glutCreateWindow("Example_1_4"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(KeyEvent);
	glutMouseFunc(MouseEvent);


	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	for (auto& e : vec_rect)
	{
		glColor3f(e.getRed(), e.getGreen(), e.getBlue());
		glRectf(e.getLeft(), e.getBottom(), e.getRight(), e.getTop());
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'a')			//�밢 �̵�
	{
		timer_a = timer_a ? false : true;
		timer_i = false;
		timer_c = false;

		if (timer_a)
			animation_num = 1;
		else
			animation_num = 0;

		glutTimerFunc(50, TimerEvent, animation_num);
	}
	else if (key == 'i')	//������� �̵�
	{
		timer_a = false;
		timer_i = timer_i ? false : true;
		timer_c = false;

		if (timer_i)
			animation_num = 2;
		else
			animation_num = 0;

		glutTimerFunc(50, TimerEvent, animation_num);
	}
	else if (key == 'c')	//ũ�� ��ȯ	
	{
		timer_a = false;
		timer_i = false;
		timer_c = timer_c ? false : true;

		if (timer_c)
			animation_num = 3;
		else
			animation_num = 0;
		glutTimerFunc(50, TimerEvent, animation_num);
	}
	else if (key == 's')	//�ִϸ��̼� ����
	{
		timer_a = false;
		timer_i = false;
		timer_c = false;
		animation_num = 0;
	}
	else if (key == 'm')	//��ġ �̵�
	{
		for (auto& e : vec_rect)
		{
			e.posReset();
		}
	}
	else if (key == 'r')	//����
	{
		vec_rect.clear();
		num_rect = 0;
		timer_a = false;
		timer_i = false;
		timer_c = false;
	}
	else if (key == 'q')
	{
		glutLeaveMainLoop();
	}

	glutPostRedisplay();
}

GLvoid MouseEvent(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (num_rect >= 5)
			return;

		vec_rect.push_back(rect(x, y));
		num_rect++;
		glutPostRedisplay();
	}
}

GLvoid TimerEvent(int value)
{
	//�밢 �̵� Ÿ�̸�
	if (value == 1 && timer_a)
	{
		for (auto& e : vec_rect)
		{
			//direct == 0 �̸� ���
			if (e.getDirect() == 0)
			{
				e.moveLeft(0.02f);
				e.moveRight(0.02f);
				e.moveBottom(0.02f);
				e.moveTop(0.02f);
				if (e.getRight() >= 1.0f && e.getTop() >= 1.0f)
				{
					e.setLeft(1.0f - e.getWidth());
					e.setRight(1.0f);
					e.setBottom(1.0f - e.getHeight());
					e.setTop(1.0f);
					e.setDirect(2);
				}
				else if (e.getRight() >= 1.0f)
				{
					e.setLeft(1.0f - e.getWidth());
					e.setRight(1.0f);
					e.setDirect(1);
				}
				else if (e.getTop() >= 1.0f)
				{
					e.setBottom(1.0f - e.getHeight());
					e.setTop(1.0f);
					e.setDirect(3);
				}
			}
			//1�̸� �»�
			else if (e.getDirect() == 1)
			{
				e.moveLeft(-0.02f);
				e.moveRight(-0.02f);
				e.moveBottom(0.02f);
				e.moveTop(0.02f);
				if (e.getLeft() <= -1.0f && e.getTop() >= 1.0f)
				{
					e.setRight(-1.0f + e.getWidth());
					e.setLeft((-1.0f));
					e.setBottom(1.0f - e.getHeight());
					e.setTop(1.0f);
					e.setDirect(3);
				}
				else if (e.getLeft() <= -1.0f)
				{
					e.setRight(-1.0f + e.getWidth());
					e.setLeft((-1.0f));
					e.setDirect(0);
				}
				if (e.getTop() >= 1.0f)
				{
					e.setBottom(1.0f - e.getHeight());
					e.setTop(1.0f);
					e.setDirect(2);
				}
			}
			//2�̸� ����
			else if (e.getDirect() == 2)
			{
				e.moveLeft(-0.02f);
				e.moveRight(-0.02f);
				e.moveBottom(-0.02f);
				e.moveTop(-0.02f);
				if (e.getLeft() <= -1.0f && e.getBottom() <= -1.0f)
				{
					e.setRight(-1.0f + e.getWidth());
					e.setLeft((-1.0f));
					e.setTop(-1.0f + e.getHeight());
					e.setBottom(-1.0f);
					e.setDirect(0);
				}
				else if (e.getLeft() <= -1.0f)
				{
					e.setRight(-1.0f + e.getWidth());
					e.setLeft((-1.0f));
					e.setDirect(3);
				}
				if (e.getBottom() <= -1.0f)
				{
					e.setTop(-1.0f + e.getHeight());
					e.setBottom(-1.0f);
					e.setDirect(1);
				}
			}
			//3�̸� ����
			else if (e.getDirect() == 3)
			{
				e.moveLeft(0.02f);
				e.moveRight(0.02f);
				e.moveBottom(-0.02f);
				e.moveTop(-0.02f);
				if (e.getRight() >= 1.0f && e.getBottom() <= -1.0f)
				{
					e.setLeft(1.0f - e.getWidth());
					e.setRight(1.0f);
					e.setTop(-1.0f + e.getHeight());
					e.setBottom(-1.0f);
					e.setDirect(1);
				}
				if (e.getRight() >= 1.0f)
				{
					e.setLeft(1.0f - e.getWidth());
					e.setRight(1.0f);
					e.setDirect(2);
				}
				if (e.getBottom() <= -1.0f)
				{
					e.setTop(-1.0f + e.getHeight());
					e.setBottom(-1.0f);
					e.setDirect(0);
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(50, TimerEvent, animation_num);
	}
	//������� �̵� Ÿ�̸�
	else if (value == 2 && timer_i)
	{
		for (auto& e : vec_rect)
		{
			//��� �̵� ���� ������ �Ʒ���
			if (e.getDirect() == 0)
			{
				e.moveLeft(0.02f);
				e.moveRight(0.02f);
				if (e.getRight() >= 1.0f)
				{
					e.setLeft(1.0f - e.getWidth());
					e.setRight(1.0f);
					e.moveBottom(-0.1f);
					e.moveTop(-0.1f);
					e.setDirect(1);
					if (e.getBottom() <= -1.0f)
					{
						e.setTop(-1.0f + e.getHeight());
						e.setBottom(-1.0f);
						e.setDirect(3);
					}
				}
			}
			//�·� �̵� ���� ������ �Ʒ���
			else if (e.getDirect() == 1)
			{
				e.moveLeft(-0.02f);
				e.moveRight(-0.02f);
				if (e.getLeft() <= -1.0f)
				{
					e.setRight(-1.0f + e.getWidth());
					e.setLeft(-1.0f);
					e.moveBottom(-0.1f);
					e.moveTop(-0.1f);
					e.setDirect(0);
					if (e.getBottom() <= -1.0f)
					{
						e.setTop(-1.0f + e.getHeight());
						e.setBottom(-1.0f);
						e.setDirect(2);
					}
				}
			}
			//��� �̵� ���� ������ ����
			else if (e.getDirect() == 2)
			{
				e.moveLeft(0.02f);
				e.moveRight(0.02f);
				if (e.getRight() >= 1.0f)
				{
					e.setLeft(1.0f - e.getWidth());
					e.setRight(1.0f);
					e.moveBottom(0.1f);
					e.moveTop(0.1f);
					e.setDirect(3);
					if (e.getTop() >= 1.0f)
					{
						e.setBottom(1.0f - e.getHeight());
						e.setTop(1.0f);
						e.setDirect(1);
					}
				}
			}
			//�·� �̵� ���� ������ ����
			else if (e.getDirect() == 3)
			{
				e.moveLeft(-0.02f);
				e.moveRight(-0.02f);
				if (e.getLeft() <= -1.0f)
				{
					e.setRight(-1.0f + e.getWidth());
					e.setLeft(-1.0f);
					e.moveBottom(0.1f);
					e.moveTop(0.1f);
					e.setDirect(2);
					if (e.getTop() >= 1.0f)
					{
						e.setBottom(1.0f - e.getHeight());
						e.setTop(1.0f);
						e.setDirect(0);
					}
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(50, TimerEvent, animation_num);
	}
	else if (value == 3 && timer_c)
	{
		for (auto& e : vec_rect)
		{
			//ũ�� �����ϰ� �ٲٰ�
			//�׿����� �̵��� ������ �ٲܶ��� �ٲ������
			std::uniform_int_distribution<int> dis_width(0, 100);
			std::uniform_int_distribution<int> dis_height(0, 100);

			GLfloat delW = static_cast<float>(dis_width(gen) - 50) * 0.001f;
			GLfloat delH = static_cast<float>(dis_width(gen) - 50) * 0.001f;

			delW = floor(delW * 100.f + 0.5) / 100.f;
			delH = floor(delH * 100.f + 0.5) / 100.f;

			if (e.getLeft() - delW < e.getRight())
			{
				e.moveLeft(-delW);
				e.moveRight(delW);
			}
			if (e.getBottom() - delH < e.getTop())
			{
				e.moveTop(delH);
				e.moveBottom(-delH);
			}
		}
		glutPostRedisplay();
		glutTimerFunc(50, TimerEvent, animation_num);
	}
}