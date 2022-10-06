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
GLuint vbo[2];

std::random_device rd;
std::mt19937 gen(rd());

std::vector<GLfloat> vertex =
{
	//2사분면
	-0.7f, 0.3f, 0.0f,
	-0.3f, 0.3f, 0.0f,
	-0.5f, 0.3f, 0.0f,//옮길 점


	//1사분면 -start 9
	0.3f, 0.3f, 0.0f,
	0.7f,0.3f,0.0f,
	0.5f,0.7f,0.0f, //겹쳐져있는 옮길점

	0.3f, 0.3f, 0.0f,
	0.5f,0.7f,0.0f,	//겹쳐져있는 옮길점
	0.5f,0.7f,0.0f, //-> 옮길 점

	//3사분면 -start 27
	-0.6f, -0.4f, 0.0f, //옮길점
	-0.6f, -0.6f, 0.0f, 
	-0.4f, -0.6f, 0.0f, 

	-0.4f, -0.6f, 0.0f, 
	-0.4f, -0.4f, 0.0f,	//옮길점
	-0.6f, -0.4f, 0.0f,	//옮길점

	-0.6f, -0.4f, 0.0f,
	-0.4f, -0.4f, 0.0f,
	-0.5f, -0.4f, 0.0f, //위로 옮길점

	//4사분면 -start 54
	0.5f, -0.4f, 0.0f,
	0.3f, -0.5f, 0.0f,
	0.4f, -0.6f, 0.0f,

	0.5f, -0.4f, 0.0f,
	0.4f, -0.6f, 0.0f,
	0.6f, -0.6f, 0.0f,

	0.5f, -0.4f, 0.0f,
	0.6f, -0.6f, 0.0f,
	0.7f, -0.5f, 0.0f
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
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
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
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f

};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_2_5(10번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
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

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	
	if (!timer && time_on == 0)
	{
		glDrawArrays(GL_LINE_STRIP, 0, 2);
		glDrawArrays(GL_TRIANGLES, 3, vertex.size() / 3);
	}
	else if (time_on == 100)
	{
		glDrawArrays(GL_TRIANGLES, 0, 18);
		glPointSize(5);
		glDrawArrays(GL_POINTS, 19, 1);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);

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
		vertex[21] += 0.003f;
		vertex[24] -= 0.003f;

		vertex[27] -= 0.001f;
		vertex[42] -= 0.001f;
		vertex[39] += 0.001f;

		vertex[45] -= 0.001f;
		vertex[48] += 0.001f;
		vertex[52] += 0.002f;

		vertex[55] -= 0.001f;
		vertex[57] += 0.002f;
		vertex[60] += 0.001f;
		vertex[61] += 0.001f;

		vertex[64] -= 0.001f;
		vertex[66] += 0.001f;
		vertex[67] += 0.001f;
		vertex[69] -= 0.001f;
		vertex[70] += 0.001f;

		vertex[73] -= 0.001f;
		vertex[75] -= 0.001f;
		vertex[76] += 0.001f;
		vertex[78] -= 0.002f;


	}
	else if (time_on > 100 && time_on < 200 && timer)
	{
		vertex[7] -= 0.003f;

		vertex[15] -= 0.003f;
		vertex[21] -= 0.003f;
		vertex[24] += 0.003f;

		vertex[27] += 0.001f;
		vertex[42] += 0.001f;
		vertex[39] -= 0.001f;

		vertex[45] += 0.001f;
		vertex[48] -= 0.001f;
		vertex[52] -= 0.002f;

		vertex[55] += 0.001f;
		vertex[57] -= 0.002f;
		vertex[60] -= 0.001f;
		vertex[61] -= 0.001f;

		vertex[64] += 0.001f;
		vertex[66] -= 0.001f;
		vertex[67] -= 0.001f;
		vertex[69] += 0.001f;
		vertex[70] -= 0.001f;

		vertex[73] += 0.001f;
		vertex[75] += 0.001f;
		vertex[76] -= 0.001f;
		vertex[78] += 0.002f;
	}

	if(timer)
		time_on = (time_on + 1) % 200;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);

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
		time_on = 1;
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