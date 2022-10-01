#define _USE_MATH_DEFINES

#include "make_Shader.h"
#include <vector>
#include <random>
#include <cmath>


GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid MouseClick(int button, int state, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);


const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;

GLboolean left_click = false;
GLfloat ox, oy;
GLint angle = 0;
GLfloat radius = 0;
GLboolean rotate = true;

GLuint vao, vao02;
GLuint vbo[2], vbo02[2];

std::random_device rd;
std::mt19937 gen(rd());

std::vector<GLfloat> vertex =
{
};

std::vector<GLfloat> color =
{
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_2_4(9번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	
	glutMouseFunc(MouseClick);
	glutTimerFunc(100, TimeEvent, 1);

	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	
	glPointSize(3.0f);
	glDrawArrays(GL_POINTS, 0, vertex.size() / 3);

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (!left_click)
		{
			vertex.clear();
			color.clear();
			angle = 0;
			radius = 0;
			rotate = true;

			std::uniform_int_distribution<int> dis(0, 5);
			rColor = static_cast<GLfloat>(dis(gen)) * 0.1f;
			gColor = static_cast<GLfloat>(dis(gen)) * 0.1f;
			bColor = static_cast<GLfloat>(dis(gen)) * 0.1f;

			convert_WindowXY_OpenglXY(x, y, ox, oy);

			left_click = true;
			vertex.push_back(ox);
			vertex.push_back(oy);
			vertex.push_back(0.0f);

			color.push_back(1.0f);
			color.push_back(1.0f);
			color.push_back(1.0f);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_DYNAMIC_DRAW);
		}
	}
}

GLvoid TimeEvent(int value)
{
	if (left_click)
	{
		if (rotate)
		{
			angle += 10;
			radius += 0.001;
			vertex.push_back(radius * (cos(M_PI * angle / 180)) + ox);
			vertex.push_back(radius * (sin(M_PI * angle / 180)) + oy);
			vertex.push_back(0.0f);

			color.push_back(1.0f);
			color.push_back(1.0f);
			color.push_back(1.0f);

			if (angle == 360 * 3)
			{
				ox += radius * 2;
				angle += 180;
				rotate = false;
			}
		}
		else
		{
			angle -= 10;
			radius -= 0.001;
			vertex.push_back(radius * (cos(M_PI * angle / 180)) + ox);
			vertex.push_back(radius * (sin(M_PI * angle / 180)) + oy);
			vertex.push_back(0.0f);

			color.push_back(1.0f);
			color.push_back(1.0f);
			color.push_back(1.0f);

			if (angle == 180)
			{
				left_click = false;
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
	}

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 1);
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