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
GLint check_mouse_point(const GLfloat& ox, const GLfloat& oy, const std::vector<GLfloat>& ver);
GLboolean isInside(const GLfloat& ox, const GLfloat& oy, const std::vector<GLfloat>& ver);


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
	glutCreateWindow("Example_2_6(11번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMotion);
	glutKeyboardFunc(KeyEvent);



	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	glBindVertexArray(vao);

	glDrawArrays(GL_LINE_LOOP, 0, vertex.size() / 3);

	glPointSize(10);
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
		//점이 어디를 눌렀는지
		//점들
		GLfloat temp_ox, temp_oy;
		convert_WindowXY_OpenglXY(x, y, temp_ox, temp_oy);

		left_click = check_mouse_point(temp_ox, temp_oy, vertex);
		if (left_click >= 0)
		{
			oldx = x;
			oldy = y;
		}

		//사각형 내부
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		left_click = -1;
	}
}

GLvoid MouseMotion(int x, int y)
{
	if (left_click == 0) //사각형 클릭
	{
		GLfloat temp_ox, temp_oy;
		GLfloat old_ox, old_oy;
		convert_WindowXY_OpenglXY(x, y, temp_ox, temp_oy);
		convert_WindowXY_OpenglXY(oldx, oldy, old_ox, old_oy);

		for (int i = 0; i < 4; ++i)
		{
			vertex[3 * i] += temp_ox - old_ox;
			vertex[3 * i + 1] += temp_oy - old_oy;
		}
		oldx = x;
		oldy = y;
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	}
	else if (left_click > 0) //왼쪽 위점 클릭
	{
		GLfloat temp_ox, temp_oy;
		convert_WindowXY_OpenglXY(x, y, temp_ox, temp_oy);
		vertex[(left_click - 1) * 3] = temp_ox;
		vertex[(left_click - 1) * 3 + 1] = temp_oy;
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	}

	glutPostRedisplay();
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

GLint check_mouse_point(const GLfloat& ox, const GLfloat& oy, const std::vector<GLfloat>& ver)
{
	for (int i = 0; i < 4; ++i)
	{
		if (ox > ver[i * 3] - 0.0125f && ox < ver[i * 3] + 0.0125f && oy > ver[i * 3 + 1] - 0.0125f && oy < ver[i * 3 + 1] + 0.0125f)
		{
			return i + 1;
		}
	}

	if (isInside(ox, oy, ver))
		return 0;

	return -1;
}

GLboolean isInside(const GLfloat& ox, const GLfloat& oy, const std::vector<GLfloat>& ver)
{
	//crosses는 점q와 오른쪽 반직선과 다각형과의 교점의 개수 
	int crosses = 0;
	for (int i = 0; i < 4; i++)
	{
		int j = (i + 1) % 4;
		//점 B가 선분 (p[i], p[j])의 y좌표 사이에 있음    
		if ((ver[i * 3 + 1] > oy) != (ver[j * 3 + 1] > oy))
		{           
			//atX는 점 B를 지나는 수평선과 선분 (p[i], p[j])의 교점     
			double atX = (ver[j * 3] - ver[i * 3]) * (oy - ver[i * 3 + 1]) / (ver[j * 3 + 1] - ver[i * 3 + 1]) + ver[i * 3];
			
			//atX가 오른쪽 반직선과의 교점이 맞으면 교점의 개수를 증가시킨다.     
			if (ox < atX)
				crosses++;
		}
	}
	return crosses % 2 > 0;
}
