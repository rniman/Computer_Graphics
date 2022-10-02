#include "make_Shader.h"
#include <vector>

//Ű���� ����� ���������� �𸣰ڴ�...

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid mouseEvent(int button, int state, int x, int y);
GLvoid KeyDown(unsigned char key,int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;

GLboolean key_a = false;
GLboolean sizeup_state = true;
GLfloat sizeup = 0.01f;

GLuint vao;
GLuint vbo[2];

std::vector<std::vector<float>> a;

std::vector<GLfloat> vertex =
{
	//1��и�
	0.6, 0.4, 0.0,
	0.4, 0.4, 0.0,
	0.5, 0.7, 0.0,

	//2��и�
	-0.6, 0.4, 0.0,
	-0.4, 0.4, 0.0,
	-0.5, 0.7, 0.0,

	-0.6, -0.7, 0.0,
	-0.4, -0.7, 0.0,
	-0.5, -0.4, 0.0,

	0.6, -0.7, 0.0,
	0.4, -0.7, 0.0,
	0.5, -0.4, 0.0
};

std::vector<GLfloat> color =
{
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,

	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0
};

GLint delete_now = 0;

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_2_1(6��)");

	glewExperimental = GL_TRUE;
	glewInit();

	//���̴� �о�ͼ� ���̴� ���α׷� �����
	
	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouseEvent);
	glutKeyboardFunc(KeyDown);

	glutMainLoop();
}



GLvoid drawScene()
{
	GLfloat rColor, gColor, bColor;

	rColor = gColor = bColor = 1.0;

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderID);

	//����� vao �ҷ�����
	glBindVertexArray(vao);

	//�ﰢ�� �׸���
	if (key_a)
	{
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glDrawArrays(GL_LINE_LOOP, 3, 3);
		glDrawArrays(GL_LINE_LOOP, 6, 3);
		glDrawArrays(GL_LINE_LOOP, 9, 3);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, vertex.size());

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid mouseEvent(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		GLfloat temp_x, temp_y;
		convert_WindowXY_OpenglXY(x, y, temp_x, temp_y);
		vertex[delete_now++] = temp_x - 0.1f - sizeup;
		vertex[delete_now++] = temp_y - 0.15f - sizeup;
		vertex[delete_now++] = 1.0;

		vertex[delete_now++] = temp_x + 0.1f + sizeup;
		vertex[delete_now++] = temp_y - 0.15f - sizeup;
		vertex[delete_now++] = 1.0;

		vertex[delete_now++] = temp_x;
		vertex[delete_now++] = temp_y + 0.15f + sizeup;
		vertex[delete_now++] = 1.0;

		if (sizeup_state)
		{
			sizeup += 0.01f;
			if (sizeup > 0.1f)
				sizeup_state = false;
		}
		else
		{
			sizeup -= 0.01f;
			if (sizeup < -0.1f)
				sizeup_state = true;
		}


		if (delete_now >= vertex.size())
			delete_now = 0;

		//���ε带 ��� ����ߵ� �����ذ��ұ�?
		//if (glIsBuffer(vbo[0]))
		//{
		//	std::cout << "���ε�" << std::endl;
		//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//}

		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	}
	glutPostRedisplay();
}

GLvoid KeyDown(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		key_a = key_a ? false : true;
		glutPostRedisplay();
	}
	else if (key == 'q')
	{
		glutExit();
	}
}

void initBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STREAM_DRAW);
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
