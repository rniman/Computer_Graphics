#define _USE_MATH_DEFINES

#include "make_Shader.h"
#include <vector>
#include <random>
#include <cmath>


GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseMotion(int x, int y);

GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);


const GLint window_w = 800, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;
GLint oldx, oldy;
GLint left_click = -1;

GLuint vao;
GLuint vbo[2];

std::random_device rd;
std::mt19937 gen(rd());

std::vector<GLfloat> vertex =
{
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f,-0.5f, 0.0f,
	0.5f, 0.5f, 0.0f

	/*-0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f*/
};


std::vector<GLfloat> color =
{
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	//0.0f,1.0f,0.0f,
	//0.0f,1.0f,0.0f
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_2_5(10��)");

	glewExperimental = GL_TRUE;
	glewInit();

	//���̴� �о�ͼ� ���̴� ���α׷� �����

	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyEvent);

	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderID);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	
	glDrawArrays(GL_LINE_LOOP, 0, vertex.size() / 3);

	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, vertex.size() / 3);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT && state == GLUT_DOWN)
	{
		//���� ��� ��������

	}
	else if (button == GLUT_LEFT && state == GLUT_UP)
	{

	}
}

GLvoid MouseMotion(int x, int y)
{
	if (left_click == 0) //�簢�� Ŭ��
	{

	}
	else if (left_click == 1) //���� ���� Ŭ��
	{

	}
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
}

void initBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy)
{
	x = static_cast<int>((window_w / 2) + ox * (window_w / 2));
	y = static_cast<int>((window_h / 2) - oy * (window_h / 2));
}

GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy)
{
	ox = (float)((x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0)));
	oy = -(float)((y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0)));
}