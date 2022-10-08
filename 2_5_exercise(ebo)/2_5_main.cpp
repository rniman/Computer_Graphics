#define _USE_MATH_DEFINES

#include "make_Shader.h"
#include <vector>
#include <random>
#include <cmath>


GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);


const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;

GLuint time_on = 0;
GLboolean timer = false;

GLuint vao;
GLuint vbo[2], ebo;

std::random_device rd;
std::mt19937 gen(rd());

std::vector<GLfloat> vertex =
{
	//2��и�
	-0.7f, 0.3f, 0.0f,
	-0.3f, 0.3f, 0.0f,
	-0.5f, 0.3f, 0.0f,//�ű� ��				2
	
	//1��и� -start 9
	0.3f, 0.3f, 0.0f,
	0.7f,0.3f,0.0f,
	0.5f,0.7f,0.0f, //�������ִ� ���������� �ű���

	0.5f,0.7f,0.0f, //-> �������� �ű� ��		6

	//3��и� -start 21
	-0.6f, -0.4f, 0.0f, //�ű���
	-0.6f, -0.6f, 0.0f, 
	-0.4f, -0.6f, 0.0f, 

	-0.4f, -0.4f, 0.0f,	//�ű���

	-0.5f, -0.4f, 0.0f, //���� �ű���		11

	//4��и� -start 36
	0.5f, -0.4f, 0.0f,
	0.3f, -0.5f, 0.0f,
	0.4f, -0.6f, 0.0f,

	0.6f, -0.6f, 0.0f,

	0.7f, -0.5f, 0.0f	//					16
};

std::vector<GLuint> index =
{
	//�� -> �ﰢ��
	0, 1, 2,

	//�ﰢ�� -> �簢��
	3, 5, 4,
	3, 5, 6,

	//�簢�� -> ������
	7, 8, 9,
	9, 10, 7,
	7, 10, 11,

	//������ -> ��
	12, 13, 14,
	12, 14, 15,
	12, 15, 16
};

std::vector<GLfloat> color =
{
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
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
	glutTimerFunc(100, TimeEvent, 1);
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
	
	if (!timer && time_on == 0)
	{
		glDrawElements(GL_LINE_STRIP, 3, GL_UNSIGNED_INT, 0); //
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
	}
	else if (time_on == 100)
	{
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), index.data() + 18, GL_STATIC_DRAW);
		glPointSize(5);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
	}
	else
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid TimeEvent(int value)
{
	if (time_on > 0 && time_on < 100 && timer)
	{
		vertex[7] += 0.003f;

		vertex[15] += 0.003f;
		vertex[18] -= 0.003f;

		vertex[21] -= 0.001f;
		vertex[30] += 0.001f;
		vertex[34] += 0.002f;

		vertex[37] -= 0.001f;
		vertex[39] += 0.002f;
		vertex[42] += 0.001f;
		vertex[43] += 0.001f;
		vertex[45] -= 0.001f;
		vertex[46] += 0.001f;
		vertex[48] -= 0.002f;

	}
	else if (time_on == 100)
		return;
	else if (time_on > 100 && time_on < 200 && timer)
	{
		vertex[7] -= 0.003f;

		vertex[15] -= 0.003f;
		vertex[18] += 0.003f;

		vertex[21] += 0.001f;
		vertex[30] -= 0.001f;
		vertex[34] -= 0.002f;

		vertex[37] += 0.001f;
		vertex[39] -= 0.002f;
		vertex[42] -= 0.001f;
		vertex[43] -= 0.001f;
		vertex[45] += 0.001f;
		vertex[46] -= 0.001f;
		vertex[48] += 0.002f;
	}

	if(timer)
		time_on = (time_on + 1) % 200;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 1);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else
	{
		timer = timer ? false : true;
	}
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

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
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