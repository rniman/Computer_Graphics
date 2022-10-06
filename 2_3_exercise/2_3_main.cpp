#include "make_Shader.h"
#include <vector>
#include <random>
#include <cmath>

//키보드 명령이 무슨뜻인지 모르겠다...

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyDown(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);
GLint whatCollisionPoint(const std::vector<GLfloat>& vec, const GLint& index);

const GLint window_w = 600, window_h = 600;
GLboolean time_o = true, time_i = true;

GLuint vao, vao02;
GLuint vbo[2], vbo02[2];

std::random_device rd;
std::mt19937 gen(rd());

GLfloat speed[6];
GLint dir[6];

std::vector<GLfloat> rect_vertex =
{
	0.5,0.5,0.0,
	0.5,-0.5,0.0,
	-0.5,-0.5,0.0,
	-0.5,0.5,0.0
};

std::vector<GLfloat> rect_color =
{
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5
};

std::vector<GLfloat> vertex =
{
	//1사분면
	0.55, 0.4, 0.0,
	0.65, 0.4, 0.0,
	0.6, 0.55, 0.0,

	0.55, -0.55, 0.0,
	0.65, -0.55, 0.0,
	0.6, -0.4, 0.0,

	-0.65, -0.55, 0.0,
	-0.55, -0.55, 0.0,
	-0.6, -0.4, 0.0,

	-0.65, 0.4, 0.0,
	-0.55, 0.4, 0.0,
	-0.6, 0.55, 0.0,

	-0.5, -0.5, 0.0,
	-0.3, -0.5, 0.0,
	-0.4, -0.2, 0.0,

	0.3, 0.5, 0.0,
	0.5, 0.5, 0.0,
	0.4, 0.2, 0.0,
};

std::vector<GLfloat> color =
{
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0
};

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_2_3(8번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	std::uniform_int_distribution<int> dis(3, 8);
	for (auto& e : speed)
		e = static_cast<GLfloat>(dis(gen)) * 0.01f;
	speed[4] = 0.04f;
	speed[5] = 0.04f;

	std::uniform_int_distribution<int> dis_dir(0, 3);
	for (auto& e : dir)
		e = dis_dir(gen);
	dir[4] = 0;
	dir[5] = 3;

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutTimerFunc(100, TimeEvent, 1);

	glutMainLoop();
}

GLvoid drawScene()
{
	GLfloat rColor, gColor, bColor;

	rColor = gColor = bColor = 0.7f;

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	//사각형 그림
	glBindVertexArray(vao02);
	glBindBuffer(GL_ARRAY_BUFFER, vbo02[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	//삼각형 그림
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertex.size());

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid KeyDown(unsigned char key, int x, int y)
{
	if (key == 'o')
	{
		time_o = time_o ? false : true;
	}
	else if (key == 'i')
	{
		time_i = time_i ? false : true;
	}
	else if (key == 'q')
	{
		glutExit();
	}
}

GLvoid TimeEvent(int value)
{

	//이동
	for (int i = 0; i < 4; ++i)
	{
		if (!time_o)
			break;
		//우상
		if (dir[i] == 0)
		{
			for (int j = 0; j < 3; ++j)
			{
				vertex[9 * i + j * 3] += speed[i];
				vertex[9 * i + j * 3 + 1] += speed[i];
			}
		}
		//좌상
		else if (dir[i] == 1)
		{
			for (int j = 0; j < 3; ++j)
			{
				vertex[9 * i + j * 3] -= speed[i];
				vertex[9 * i + j * 3 + 1] += speed[i];
			}
		}
		//좌하
		else if (dir[i] == 2)
		{
			for (int j = 0; j < 3; ++j)
			{
				vertex[9 * i + j * 3] -= speed[i];
				vertex[9 * i + j * 3 + 1] -= speed[i];
			}
		}
		//우하
		else if (dir[i] == 3)
		{
			for (int j = 0; j < 3; ++j)
			{
				vertex[9 * i + j * 3] += speed[i];
				vertex[9 * i + j * 3 + 1] -= speed[i];
			}
		}

		GLint col_point = whatCollisionPoint(vertex, i);
		if (col_point != -1)
		{
			if (dir[i] == 0 && vertex[i * 9 + col_point * 3] - speed[i] < -0.5f)
			{
				dir[i] = 1;
				vertex[i * 9] = -0.5f;
				vertex[i * 9 + 1] = vertex[i * 9 + 7] - 0.05f;
				vertex[i * 9 + 3] = -0.5f;
				vertex[i * 9 + 4] = vertex[i * 9 + 7] + 0.05f;
				vertex[i * 9 + 6] = -0.65f;
			}
			else if (dir[i] == 0 && vertex[i * 9 + col_point * 3 + 1] - speed[i] <= -0.5f)
			{
				dir[i] = 3;
				vertex[i * 9] = vertex[i * 9 + 6] + 0.05f;
				vertex[i * 9 + 1] = -0.5f;
				vertex[i * 9 + 3] = vertex[i * 9 + 6] - 0.05f;
				vertex[i * 9 + 4] = -0.5f;
				vertex[i * 9 + 7] = -0.65f;
			}
			else if (dir[i] == 1 && vertex[i * 9 + col_point * 3] + speed[i] > 0.5f)
			{
				dir[i] = 0;
				vertex[i * 9] = 0.5f;
				vertex[i * 9 + 1] = vertex[i * 9 + 7] + 0.05f;
				vertex[i * 9 + 3] = 0.5f;
				vertex[i * 9 + 4] = vertex[i * 9 + 7] - 0.05f;
				vertex[i * 9 + 6] = 0.65f;
			}
			else if (dir[i] == 1 && vertex[i * 9 + col_point * 3 + 1] - speed[i] <= -0.5f)
			{
				dir[i] = 2;
				vertex[i * 9] = vertex[i * 9 + 6] + 0.05f;
				vertex[i * 9 + 1] = -0.5f;
				vertex[i * 9 + 3] = vertex[i * 9 + 6] - 0.05f;
				vertex[i * 9 + 4] = -0.5f;
				vertex[i * 9 + 7] = -0.65f;
			}
			else if (dir[i] == 2 && vertex[i * 9 + col_point * 3] + speed[i] > 0.5f)
			{
				dir[i] = 3;
				vertex[i * 9] = 0.5f;
				vertex[i * 9 + 1] = vertex[i * 9 + 7] + 0.05f;
				vertex[i * 9 + 3] = 0.5f;
				vertex[i * 9 + 4] = vertex[i * 9 + 7] - 0.05f;
				vertex[i * 9 + 6] = 0.65f;
			}
			else if (dir[i] == 2 && vertex[i * 9 + col_point * 3 + 1] + speed[i] >= 0.5f)
			{
				dir[i] = 1;
				vertex[i * 9] = vertex[i * 9 + 6] - 0.05f;
				vertex[i * 9 + 1] = 0.5f;
				vertex[i * 9 + 3] = vertex[i * 9 + 6] + 0.05f;
				vertex[i * 9 + 4] = 0.5f;
				vertex[i * 9 + 7] = 0.65f;
			}
			else if (dir[i] == 3 && vertex[i * 9 + col_point * 3] - speed[i] < -0.5f)
			{
				dir[i] = 2;
				vertex[i * 9] = -0.5f;
				vertex[i * 9 + 1] = vertex[i * 9 + 7] - 0.05f;
				vertex[i * 9 + 3] = -0.5f;
				vertex[i * 9 + 4] = vertex[i * 9 + 7] + 0.05f;
				vertex[i * 9 + 6] = -0.65f;
			}
			else if (dir[i] == 3 && vertex[i * 9 + col_point * 3 + 1] + speed[i] >= 0.5f)
			{
				dir[i] = 0;
				vertex[i * 9] = vertex[i * 9 + 6] - 0.05f;
				vertex[i * 9 + 1] = 0.5f;
				vertex[i * 9 + 3] = vertex[i * 9 + 6] + 0.05f;
				vertex[i * 9 + 4] = 0.5f;
				vertex[i * 9 + 7] = 0.65f;
			}
		}

		//우상
		if (dir[i] == 0 && ((vertex[9 * i] >= 1.0f) || vertex[9 * i + 3] >= 1.0f || (vertex[9 * i + 6] >= 1.0f)))
		{
			dir[i] = 1;
			vertex[9 * i] = 1.0f;
			vertex[9 * i + 1] = vertex[9 * i + 7] - 0.05f;
			vertex[9 * i + 3] = 1.0f;
			vertex[9 * i + 4] = vertex[9 * i + 7] + 0.05f;
			vertex[9 * i + 6] = 0.85f;
		}
		else if (dir[i] == 0 && ((vertex[9 * i + 1] >= 1.0f) || (vertex[9 * i + 4] >= 1.0f) || (vertex[9 * i + 7] >= 1.0f)))
		{
			dir[i] = 3;
			vertex[9 * i] = vertex[9 * i + 6] + 0.05f;
			vertex[9 * i + 1] = 1.0f;
			vertex[9 * i + 3] = vertex[9 * i + 6] - 0.05f;
			vertex[9 * i + 4] = 1.0f;
			vertex[9 * i + 7] = 0.85f;
		}
		//좌상
		else if (dir[i] == 1 && ((vertex[9 * i] <= -1.0f) || vertex[9 * i + 3] <= -1.0f || (vertex[9 * i + 6] <= -1.0f)))
		{
			dir[i] = 0;
			vertex[9 * i] = -1.0f;
			vertex[9 * i + 1] = vertex[9 * i + 7] + 0.05f;
			vertex[9 * i + 3] = -1.0f;
			vertex[9 * i + 4] = vertex[9 * i + 7] - 0.05f;
			vertex[9 * i + 6] = -0.85;
		}
		else if (dir[i] == 1 && ((vertex[9 * i + 1] >= 1.0f) || (vertex[9 * i + 4] >= 1.0f) || (vertex[9 * i + 7] >= 1.0f)))
		{
			dir[i] = 2;
			vertex[9 * i] = vertex[9 * i + 6] + 0.05f;
			vertex[9 * i + 1] = 1.0f;
			vertex[9 * i + 3] = vertex[9 * i + 6] - 0.05f;
			vertex[9 * i + 4] = 1.0f;
			vertex[9 * i + 7] = 0.85f;
		}
		//좌하
		else if (dir[i] == 2 && ((vertex[9 * i] <= -1.0f) || vertex[9 * i + 3] <= -1.0f || (vertex[9 * i + 6] <= -1.0f)))
		{
			dir[i] = 3;
			vertex[9 * i] = -1.0f;
			vertex[9 * i + 1] = vertex[9 * i + 7] + 0.05f;
			vertex[9 * i + 3] = -1.0f;
			vertex[9 * i + 4] = vertex[9 * i + 7] - 0.05f;
			vertex[9 * i + 6] = -0.85;
		}
		else if (dir[i] == 2 && ((vertex[9 * i + 1] <= -1.0f) || (vertex[9 * i + 4] <= -1.0f) || (vertex[9 * i + 7] <= -1.0f)))
		{
			dir[i] = 1;
			vertex[9 * i] = vertex[9 * i + 6] - 0.05f;
			vertex[9 * i + 1] = -1.0f;
			vertex[9 * i + 3] = vertex[9 * i + 6] + 0.05f;
			vertex[9 * i + 4] = -1.0f;
			vertex[9 * i + 7] = -0.85f;
		}
		//우하
		else if (dir[i] == 3 && ((vertex[9 * i] >= 1.0f) || vertex[9 * i + 3] >= 1.0f || (vertex[9 * i + 6] >= 1.0f)))
		{
			dir[i] = 2;
			vertex[9 * i] = 1.0f;
			vertex[9 * i + 1] = vertex[9 * i + 7] - 0.05f;
			vertex[9 * i + 3] = 1.0f;
			vertex[9 * i + 4] = vertex[9 * i + 7] + 0.05f;
			vertex[9 * i + 6] = 0.85f;
		}
		else if (dir[i] == 3 && ((vertex[9 * i + 1] <= -1.0f) || (vertex[9 * i + 4] <= -1.0f) || (vertex[9 * i + 7] <= -1.0f)))
		{
			dir[i] = 0;
			vertex[9 * i] = vertex[9 * i + 6] - 0.05f;
			vertex[9 * i + 1] = -1.0f;
			vertex[9 * i + 3] = vertex[9 * i + 6] + 0.05f;
			vertex[9 * i + 4] = -1.0f;
			vertex[9 * i + 7] = -0.85f;
		}
	}

	std::uniform_int_distribution<int> dis(0, 10);
	GLfloat red = static_cast<GLfloat>(dis(gen)) * 0.1f;
	GLfloat green = static_cast<GLfloat>(dis(gen)) * 0.1f;
	GLfloat blue = static_cast<GLfloat>(dis(gen)) * 0.1f;
	for (int i = 4; i < 6; ++i)
	{
		if (!time_i)
			break;
		if (dir[i] == 0)
		{
			for(int j=0 ;j<3 ;++j)
				vertex[9 * i + j * 3] += speed[i];
			if (vertex[9 * i + 3] >= 0.5f)
			{
				dir[i] = 1;
				vertex[9 * i] = 0.3f;
				vertex[9 * i + 3] = 0.5f;
				vertex[9 * i + 6] = 0.4f;
				
				vertex[9 * i + 1] += speed[i];
				vertex[9 * i + 4] += speed[i];
				vertex[9 * i + 7] += speed[i];

				//std::uniform_int_distribution<int> dis(0, 10);
				//GLfloat red = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat green = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat blue = static_cast<GLfloat>(dis(gen)) * 0.1f;

				for (int j = 0; j < 3; ++j)
				{
					color[9 * i + j * 3] = red;
					color[9 * i + j * 3 + 1] = green;
					color[9 * i + j * 3 + 2] = blue;
				}
				
				if (vertex[9 * i + 7] >= 0.5f)
				{
					dir[i] = 3;
					vertex[9 * i + 1] = 0.5f;
					vertex[9 * i + 4] = 0.5f;
					vertex[9 * i + 7] = 0.2f;
				}
			}
		}
		else if (dir[i] == 1)
		{
			for (int j = 0; j < 3; ++j)
				vertex[9 * i + j * 3] -= speed[i];
			if (vertex[9 * i] <= -0.5f)
			{
				dir[i] = 0;
				vertex[9 * i] = -0.5f;
				vertex[9 * i + 3] = -0.3f;
				vertex[9 * i + 6] = -0.4f;

				vertex[9 * i + 1] += speed[i];
				vertex[9 * i + 4] += speed[i];
				vertex[9 * i + 7] += speed[i];

				//std::uniform_int_distribution<int> dis(0, 10);
				//GLfloat red = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat green = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat blue = static_cast<GLfloat>(dis(gen)) * 0.1f;

				for (int j = 0; j < 3; ++j)
				{
					color[9 * i + j * 3] = red;
					color[9 * i + j * 3 + 1] = green;
					color[9 * i + j * 3 + 2] = blue;
				}

				if (vertex[9 * i + 7] >= 0.5f)
				{
					dir[i] = 2;
					vertex[9 * i + 1] = 0.5f;
					vertex[9 * i + 4] = 0.5f;
					vertex[9 * i + 7] = 0.2f;
				}
			}
		}
		else if (dir[i] == 2)
		{
			for (int j = 0; j < 3; ++j)
				vertex[9 * i + j * 3] += speed[i];
			if (vertex[9 * i + 3] >= 0.5f)
			{
				dir[i] = 3;
				vertex[9 * i] = 0.3f;
				vertex[9 * i + 3] = 0.5f;
				vertex[9 * i + 6] = 0.4f;

				vertex[9 * i + 1] -= speed[i];
				vertex[9 * i + 4] -= speed[i];
				vertex[9 * i + 7] -= speed[i];

				//std::uniform_int_distribution<int> dis(0, 10);
				//GLfloat red = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat green = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat blue = static_cast<GLfloat>(dis(gen)) * 0.1f;

				for (int j = 0; j < 3; ++j)
				{
					color[9 * i + j * 3] = red;
					color[9 * i + j * 3 + 1] = green;
					color[9 * i + j * 3 + 2] = blue;
				}

				if (vertex[9 * i + 7] <= -0.5f)
				{
					dir[i] = 1;
					vertex[9 * i + 1] = -0.5f;
					vertex[9 * i + 4] = -0.5f;
					vertex[9 * i + 7] = -0.2f;
				}
			}
		}
		else if (dir[i] == 3)
		{
			for (int j = 0; j < 3; ++j)
				vertex[9 * i + j * 3] -= speed[i];
			if (vertex[9 * i] <= -0.5f)
			{
				dir[i] = 2;
				vertex[9 * i] = -0.5f;
				vertex[9 * i + 3] = -0.3f;
				vertex[9 * i + 6] = -0.4f;

				vertex[9 * i + 1] -= speed[i];
				vertex[9 * i + 4] -= speed[i];
				vertex[9 * i + 7] -= speed[i];

				//std::uniform_int_distribution<int> dis(0, 10);
				//GLfloat red = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat green = static_cast<GLfloat>(dis(gen)) * 0.1f;
				//GLfloat blue = static_cast<GLfloat>(dis(gen)) * 0.1f;

				for (int j = 0; j < 3; ++j)
				{
					color[9 * i + j * 3] = red;
					color[9 * i + j * 3 + 1] = green;
					color[9 * i + j * 3 + 2] = blue;
				}

				if (vertex[9 * i + 7] <= -0.5f)
				{
					dir[i] = 0;
					vertex[9 * i + 1] = -0.5f;
					vertex[9 * i + 4] = -0.5f;
					vertex[9 * i + 7] = -0.2f;
				}
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
	glutPostRedisplay();

	glutTimerFunc(100, TimeEvent, 1);
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

	//사각형 내부 그릴껏
	glGenVertexArrays(1, &vao02);
	glBindVertexArray(vao02);
	glGenBuffers(2, vbo02);

	glBindBuffer(GL_ARRAY_BUFFER, vbo02[1]);
	glBufferData(GL_ARRAY_BUFFER, rect_color.size() * sizeof(GLfloat), rect_color.data(), GL_STREAM_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo02[0]);
	glBufferData(GL_ARRAY_BUFFER, rect_vertex.size() * sizeof(GLfloat), rect_vertex.data(), GL_STATIC_DRAW);
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

GLint whatCollisionPoint(const std::vector<GLfloat>& vec, const GLint& index)
{
	if (vec[index * 9] > -0.5f && vec[index * 9 + 1] > -0.5f && vec[index * 9] < 0.5f && vec[index * 9 + 1] < 0.5f)
	{
		return 0;
	}
	else if (vec[index * 9 + 3] > -0.5f && vec[index * 9 + 4] > -0.5f && vec[index * 9 + 3] < 0.5f && vec[index * 9 + 4] < 0.5f)
	{
		return 1;
	}
	else if (vec[index * 9 + 6] > -0.5f && vec[index * 9 + 7] > -0.5f && vec[index * 9 + 6] < 0.5f && vec[index * 9 + 7] < 0.5f)
	{
		return 1;
	}
	else
		return -1;
}