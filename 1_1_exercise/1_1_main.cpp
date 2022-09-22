#include <iostream>
#include <random>
#include <cmath>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;
GLboolean timer_on = false;

std::random_device rd;
std::mt19937 gen(rd());

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ 
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 50); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
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
	//glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value)
{
	if (timer_on)
	{
		std::uniform_int_distribution<int> dis(0, 100);
		red = static_cast<float>(dis(gen)) * 0.01f;
		red = floor(red * 100) / 100;
		green = static_cast<float>(dis(gen)) * 0.01f;
		green = floor(green * 100) / 100;
		blue = static_cast<float>(dis(gen)) * 0.01f;
		blue = floor(blue * 100) / 100;
		
		glutPostRedisplay();
		glutTimerFunc(500, TimerFunction, 1);
	}
	else
	{

	}
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'r')			//������
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	else if (key == 'g')	//�ʷϻ�
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	else if (key == 'b')	//�Ķ���
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
	}
	else if (key == 'w')	//���
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
	}
	else if (key == 'k')	//������
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	else if (key == 'a')	//������
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		//�Ҽ��� 3��° ����
		std::uniform_int_distribution<int> dis(0, 100);
		red = static_cast<float>(dis(gen)) * 0.01f;
		red = floor(red * 100) / 100;
		green = static_cast<float>(dis(gen)) * 0.01f;
		green = floor(green * 100) / 100;
		blue = static_cast<float>(dis(gen)) * 0.01f;
		blue = floor(blue * 100) / 100;
	}
	else if (key == 't')	//Ÿ�̸� ȣ�� ����
	{
		if (!timer_on)
		{
			timer_on = timer_on ? false : true;
			glutTimerFunc(500, TimerFunction, 1);
		}
	}
	else if (key == 's')	//Ÿ�̸� ����
	{
		if (timer_on)
		{
			timer_on = timer_on ? false : true;
			glutTimerFunc(500, TimerFunction, 1);
		}
	}
	else if (key == 'q')		//����
	{
		glutLeaveMainLoop();
	}
	else
	{
		return;
	}

	//������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh
	glutPostRedisplay();
}