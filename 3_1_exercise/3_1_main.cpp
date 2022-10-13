#define _USE_MATH_DEFINES

#include "make_Shader.h"
#include <vector>

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


const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;
GLboolean hexa_on[6] = { false, };
GLboolean tetra_on[6] = { false, };

GLuint vao[3];
GLuint vbo_axes[2];
GLuint vbo[2];
GLuint vbo_tetra[2];
unsigned int modelLocation;

glm::mat4 coordinate_axes(1.0f);

std::vector<GLfloat> axes =
{
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,-1.0f,0.0f
};

std::vector<GLfloat> hexa_vertex =
{
	//�޸�
	-0.3f,0.3f,0.3f,
	-0.3f,-0.3f,0.3f,
	0.3f,0.3f,0.3f,

	-0.3f,-0.3f,0.3f,
	0.3f,-0.3f,0.3f,
	0.3f,0.3f,0.3f,

	//���ʸ�
	0.3f,0.3f,0.3f,
	0.3f,-0.3f,0.3f,
	0.3f,0.3f,-0.3f,

	0.3f,-0.3f,0.3f,
	0.3f,-0.3f,-0.3f,
	0.3f,0.3f,-0.3f,

	//�ո�
	0.3f,0.3f,-0.3f,
	0.3f,-0.3f,-0.3f,
	-0.3f,0.3f,-0.3f,

	0.3f,-0.3f,-0.3f,
	-0.3f,-0.3f,-0.3f,
	-0.3f,0.3f,-0.3f,

	//�����ʸ�
	-0.3f,0.3f,-0.3f,
	-0.3f,-0.3f,-0.3f,
	-0.3f,0.3f,0.3f,

	-0.3f,-0.3f,-0.3f,
	-0.3f,-0.3f,0.3f,
	-0.3f,0.3f,0.3f,

	//����
	-0.3f,0.3f,-0.3f,
	-0.3f,0.3f,0.3f,
	0.3f,0.3f,-0.3f,

	-0.3f,0.3f,0.3f,
	0.3f,0.3f,0.3f,
	0.3f,0.3f,-0.3f,

	//�Ʒ���
	-0.3f,-0.3f,0.3f,
	-0.3f,-0.3f,-0.3f,
	0.3f,-0.3f,0.3f,

	-0.3f,-0.3f,-0.3f,
	0.3f,-0.3f,-0.3f,
	0.3f,-0.3f,0.3f,
};

std::vector<GLfloat> color_2 =
{
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f
};

std::vector<GLfloat> hexa_color =
{
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
		
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
		
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
		
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
		
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
		
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
};

std::vector<GLfloat> tetra_vertex =
{
	//�ظ�
	0.0f, 0.0f, 0.2f,
	-0.1f, 0.0f, -0.1f,
	0.1f, 0.0f, -0.1f,

	//�޸�
	-0.1f, 0.0f, -0.1f,
	0.1f, 0.0f, -0.1f,
	0.0f, 0.4f, 0.0f,

	//���ʸ�
	0.1f, 0.0f, -0.1f,
	0.0f, 0.0f, 0.2f,
	0.0f, 0.4f, 0.0f,

	//�����ʸ�
	0.0f, 0.0f, 0.2f,
	-0.1f, 0.0f, -0.1f,
	0.0f, 0.4f, 0.0f,
};

std::vector<GLfloat> tetra_color =
{
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,

	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,

	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_1(12��)");

	glewExperimental = GL_TRUE;
	glewInit();

	//���̴� �о�ͼ� ���̴� ���α׷� �����

	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMotion);
	glutKeyboardFunc(KeyEvent);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderID);
	
	glm::mat4 Rz = glm::mat4(1.0f);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	//��ǥ�� �׸���
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(coordinate_axes));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 4);

	//����ü �׸���
	Rz = glm::rotate(Rz, glm::radians(-10.0f), glm::vec3(1.0, 0.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(-10.0f), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rz));
	glBindVertexArray(vao[1]);
	
	for (int i = 0; i < 6; ++i)
	{
		if (hexa_on[i])
		{
			glDrawArrays(GL_TRIANGLES, 6 * i, 6);
		}
			
	}

	glBindVertexArray(vao[2]);

	for (int i = 0; i < 4; ++i)
	{
		if (tetra_on[i])
		{
			glDrawArrays(GL_TRIANGLES, 3 * i, 3);
		}
	}

	//glDrawArrays(GL_TRIANGLES, 0, hexa_vertex.size() / 3);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid MouseClick(int button, int state, int x, int y)
{

}

GLvoid MouseMotion(int x, int y)
{

	glutPostRedisplay();
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == '1')
	{
		hexa_on[0] = hexa_on[0] ? false : true;
	}
	else if (key == '2')
	{
		hexa_on[1] = hexa_on[1] ? false : true;
	}
	else if (key == '3')
	{
		hexa_on[2] = hexa_on[2] ? false : true;
	}
	else if (key == '4')
	{
		hexa_on[3] = hexa_on[3] ? false : true;
	}
	else if (key == '5')
	{
		hexa_on[4] = hexa_on[4] ? false : true;
	}
	else if (key == '6')
	{
		hexa_on[5] = hexa_on[5] ? false : true;
	}
	else if (key == '7')
	{
		tetra_on[0] = tetra_on[0] ? false : true;
	}
	else if (key == '8')
	{
		tetra_on[1] = tetra_on[1] ? false : true;
	}
	else if (key == '9')
	{
		tetra_on[2] = tetra_on[2] ? false : true;
	}
	else if (key == '0')
	{
		tetra_on[3] = tetra_on[3] ? false : true;
	}
	else if (key == 'a')
	{
		hexa_on[0] = true;
		hexa_on[2] = true;
		hexa_on[1] = false;
		hexa_on[3] = false;
		hexa_on[4] = false;
		hexa_on[5] = false;
	}
	else if (key == 'b')
	{
		hexa_on[1] = true;
		hexa_on[3] = true;
		hexa_on[0] = false;
		hexa_on[2] = false;
		hexa_on[4] = false;
		hexa_on[5] = false;
	}
	else if (key == 'c')
	{
		hexa_on[4] = true;
		hexa_on[5] = true;
		hexa_on[0] = false;
		hexa_on[1] = false;
		hexa_on[2] = false;
		hexa_on[3] = false;
	}
	else if (key == 'e')
	{
		tetra_on[0] = true;
		tetra_on[1] = true;
		tetra_on[2] = false;
		tetra_on[3] = false;
	}
	else if (key == 'f')
	{
		tetra_on[0] = true;
		tetra_on[2] = true;
		tetra_on[1] = false;
		tetra_on[3] = false;
	}
	else if (key == 'g')
	{
		tetra_on[0] = true;
		tetra_on[3] = true;
		tetra_on[1] = false;
		tetra_on[2] = false;
	}
	glutPostRedisplay();
}

void initBuffer()
{
	glGenVertexArrays(3, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(2, vbo_axes);
	glGenBuffers(2, vbo);
	glGenBuffers(2, vbo_tetra);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[1]);
	glBufferData(GL_ARRAY_BUFFER, color_2.size() * sizeof(GLfloat), color_2.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.size() * sizeof(GLfloat), axes.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, hexa_color.size() * sizeof(GLfloat), hexa_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, hexa_vertex.size() * sizeof(GLfloat), hexa_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_tetra[1]);
	glBufferData(GL_ARRAY_BUFFER, tetra_color.size() * sizeof(GLfloat), tetra_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_tetra[0]);
	glBufferData(GL_ARRAY_BUFFER, tetra_vertex.size() * sizeof(GLfloat), tetra_vertex.data(), GL_STATIC_DRAW);
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
	//crosses�� ��q�� ������ �������� �ٰ������� ������ ���� 
	int crosses = 0;
	for (int i = 0; i < 4; i++)
	{
		int j = (i + 1) % 4;
		//�� B�� ���� (p[i], p[j])�� y��ǥ ���̿� ����    
		if ((ver[i * 3 + 1] > oy) != (ver[j * 3 + 1] > oy))
		{
			//atX�� �� B�� ������ ���򼱰� ���� (p[i], p[j])�� ����     
			double atX = (ver[j * 3] - ver[i * 3]) * (oy - ver[i * 3 + 1]) / (ver[j * 3 + 1] - ver[i * 3 + 1]) + ver[i * 3];

			//atX�� ������ ���������� ������ ������ ������ ������ ������Ų��.     
			if (ox < atX)
				crosses++;
		}
	}
	return crosses % 2 > 0;
}
