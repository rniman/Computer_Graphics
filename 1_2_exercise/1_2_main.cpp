#include <iostream>
#include <random>
#include <cmath>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

const GLint window_w = 800;
const GLint window_h = 600;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid MouseEvent(int button, int state, int x, int y);
void convert_WindowXY_OpenglXY(const int& x,const int& y, float* ox, float* oy);

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;

std::random_device rd;
std::mt19937 gen(rd());

class Center_Rect
{
public:
	GLint left, right;
	GLint top, bottom;
	GLfloat red = 0.0f;
	GLfloat green = 0.0f;
	GLfloat blue = 0.0f;
	
	GLfloat glLeft, glRight;
	GLfloat glTop, glBottom;

	//gl��ǥ set
	void setGLPos();
};

void Center_Rect::setGLPos()
{
	convert_WindowXY_OpenglXY(left, bottom, &glLeft, &glBottom);
	convert_WindowXY_OpenglXY(right, top, &glRight, &glTop);
}

Center_Rect cenRect = { 300,500,200,400 };

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 50); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example_1_2"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	cenRect.setGLPos();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(MouseEvent);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	glColor3f(cenRect.red, cenRect.green, cenRect.blue);
	glRectf(cenRect.glLeft, cenRect.glBottom, cenRect.glRight, cenRect.glTop);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid MouseEvent(int button, int state, int x, int y)
{
	//x,y�� ������ ��ǥ���� �޴´� 
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//����Ŭ��
		//1.�簢�� ���� Ŭ��
		if(x < cenRect.right && x > cenRect.left && y > cenRect.top && y < cenRect.bottom)
		{ 
			std::uniform_int_distribution<int> dis(0, 100);
			cenRect.red = static_cast<float>(dis(gen)) * 0.01f;
			cenRect.red = floor(cenRect.red * 100) / 100;
			cenRect.green = static_cast<float>(dis(gen)) * 0.01f;
			cenRect.green = floor(cenRect.green * 100) / 100;
			cenRect.blue = static_cast<float>(dis(gen)) * 0.01f;
			cenRect.blue = floor(cenRect.blue * 100) / 100;
		}
		//2.�簢�� �ܺ� Ŭ��
		else
		{
			std::uniform_int_distribution<int> dis(0, 100);
			red = static_cast<float>(dis(gen)) * 0.01f;
			red = floor(red * 100) / 100;
			green = static_cast<float>(dis(gen)) * 0.01f;
			green = floor(green * 100) / 100;
			blue = static_cast<float>(dis(gen)) * 0.01f;
			blue = floor(blue * 100) / 100;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//������Ŭ��
		//1.�簢�� ���� Ŭ��
		if (x < cenRect.right && x > cenRect.left && y > cenRect.top && y < cenRect.bottom)
		{
			if (cenRect.top <= 10 || cenRect.left <= 10)
				return;

			cenRect.left -= 10;
			cenRect.right += 10;
			cenRect.top -= 10;
			cenRect.bottom += 10;

			cenRect.setGLPos();
		}
		//2.�簢�� �ܺ� Ŭ��
		else
		{
			if (cenRect.right - cenRect.left <= 20)
				return;

			cenRect.left += 10;
			cenRect.right -= 10;
			cenRect.top += 10;
			cenRect.bottom -= 10;

			cenRect.setGLPos();
		}
	}

	glutPostRedisplay();
}

//������ ��ǥ�迡 ������� ���� GL��ǥ��� �ٲٴ� �Լ�
void convert_WindowXY_OpenglXY(const int& x,const int& y, float* ox, float* oy)
{
	*ox = (float)((x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0)));
	*oy = -(float)((y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0)));
}