#include "rect.h"

const GLint objRectNum = 20;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseMove(int x, int y);

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;
GLboolean left_click = false;

std::random_device rd;
std::mt19937 gen(rd());


my_Rect myRect;
obj_Rect rectArr[objRectNum];
GLint collisionNum = 0;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 50); // �������� ��ġ ����
	glutInitWindowSize(window_w, window_h); // �������� ũ�� ����
	glutCreateWindow("Example_1_5"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	std::uniform_int_distribution<int> dis_x(0, 800);
	std::uniform_int_distribution<int> dis_y(0, 600);
	std::uniform_int_distribution<int> dis(0, 100);

	for (auto& e : rectArr)
	{
		e.setRect(dis_x(gen), dis_y(gen));
		e.setColor(dis(gen) * 0.01, dis(gen) * 0.01, dis(gen) * 0.01);
	}

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	for (auto& e : rectArr)
	{
		if (e.getState())
		{
			glColor3f(e.getRed(), e.getGreen(), e.getBlue());
			glRectf(e.getX(), e.getY(), e.getX() + e.getWidth(), e.getY() + e.getHeight());
		}
	}
	if (myRect.getState())
	{
		glColor3f(myRect.getRed(), myRect.getGreen(), myRect.getBlue());
		glRectf(myRect.getX(), myRect.getY(), myRect.getX() + myRect.getWidth(), myRect.getY() + myRect.getHeight());
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (!left_click)
		{
			myRect.setRect(x, y);
		}
		left_click = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		left_click = false;
		myRect.setState(false);
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		collisionNum = 0;
		std::uniform_int_distribution<int> dis_x(0, 800);
		std::uniform_int_distribution<int> dis_y(0, 600);
		std::uniform_int_distribution<int> dis(0, 100);

		for (auto& e : rectArr)
		{
			e.setRect(dis_x(gen), dis_y(gen));
			e.setColor(dis(gen) * 0.01, dis(gen) * 0.01, dis(gen) * 0.01);
		}
	}

	glutPostRedisplay();
}

GLvoid MouseMove(int x, int y)
{
	if (left_click == true)
	{
		myRect.setRect(x, y);
		for (auto& e : rectArr)
		{
			if (collisionNum >= objRectNum)
				break;
			if (!e.getState())
				continue;
			if (collision(myRect, e))
			{
				e.setState(false);
				collisionNum++;
			}
		}
	}
	glutPostRedisplay();
}

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy)
{
	x = static_cast<int>((window_w / 2) + ox * (window_w / 2));
	y = static_cast<int>((window_h / 2) - oy * (window_h / 2));
}

GLvoid convert_WindowXY_OpenglXY(const int& x,const int& y, float& ox, float& oy)
{
	ox = (float)((x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0)));
	oy = -(float)((y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0)));
}