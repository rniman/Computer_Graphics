#include <iostream>
#include <random>
#include <vector>
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

	GLfloat red;
	GLfloat blue;
	GLfloat green;

	GLboolean move = false;
public:
	rect()
	{
		red = (static_cast<float>(dis(gen)) * 0.01);
		green = (static_cast<float>(dis(gen)) * 0.01);
		blue = (static_cast<float>(dis(gen)) * 0.01);
	}

	rect(const int& x,const int& y)
	{
		red = (static_cast<float>(dis(gen)) * 0.01);
		green = (static_cast<float>(dis(gen)) * 0.01);
		blue = (static_cast<float>(dis(gen)) * 0.01);

		//�⺻ ũ�� 100
		convert_WindowXY_OpenglXY(x - 50, y + 50, left, bottom);
		convert_WindowXY_OpenglXY(x + 50, y - 50, right, top);
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 50); // �������� ��ġ ����
	glutInitWindowSize(window_w, window_h); // �������� ũ�� ����
	glutCreateWindow("Example_1_1"); // ������ ����

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

GLvoid KeyEvent(unsigned char key,int x, int y)
{
	if (key == 'a')
	{

	}
	else if (key == 'i')
	{

	}
	else if (key == 'c')
	{

	}
	else if (key == 's')
	{
		glutTimerFunc(500, TimerEvent, 0);
	}
	else if (key == 'm')
	{

	}
	else if (key == 'r')
	{
		vec_rect.clear();
		num_rect = 0;
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
	if (value == 1)
	{

		glutTimerFunc(500, TimerEvent, 1);
	}
	else if (value == 2)
	{

		glutTimerFunc(500, TimerEvent, 2);

	}
	else if (value == 3)
	{

		glutTimerFunc(500, TimerEvent, 3);

	}
	else if(value == 0)
	{
		return;
	}
}