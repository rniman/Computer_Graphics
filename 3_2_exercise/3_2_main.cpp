#define _USE_MATH_DEFINES

#include "axes.h"

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
GLboolean poly_state = true;
GLboolean culling_on = false;
GLboolean hexa_on[6] = { false, };
GLboolean tetra_on[6] = { false, };

GLuint vao[3];
GLuint vbo_axes[2];
GLuint vbo[2];
GLuint vbo_tetra[2];
unsigned int modelLocation;

axes_coordination axes;



std::vector<GLfloat> hexa_vertex =
{
	//뒷면
	-0.3f,0.3f,0.3f,
	-0.3f,-0.3f,0.3f,
	0.3f,0.3f,0.3f,

	-0.3f,-0.3f,0.3f,
	0.3f,-0.3f,0.3f,
	0.3f,0.3f,0.3f,

	//왼쪽면
	0.3f,0.3f,0.3f,
	0.3f,-0.3f,0.3f,
	0.3f,0.3f,-0.3f,

	0.3f,-0.3f,0.3f,
	0.3f,-0.3f,-0.3f,
	0.3f,0.3f,-0.3f,

	//앞면
	0.3f,0.3f,-0.3f,
	0.3f,-0.3f,-0.3f,
	-0.3f,0.3f,-0.3f,

	0.3f,-0.3f,-0.3f,
	-0.3f,-0.3f,-0.3f,
	-0.3f,0.3f,-0.3f,

	//오른쪽면
	-0.3f,0.3f,-0.3f,
	-0.3f,-0.3f,-0.3f,
	-0.3f,0.3f,0.3f,

	-0.3f,-0.3f,-0.3f,
	-0.3f,-0.3f,0.3f,
	-0.3f,0.3f,0.3f,

	//윗면
	-0.3f,0.3f,-0.3f,
	-0.3f,0.3f,0.3f,
	0.3f,0.3f,-0.3f,

	-0.3f,0.3f,0.3f,
	0.3f,0.3f,0.3f,
	0.3f,0.3f,-0.3f,

	//아랫면
	-0.3f,-0.3f,0.3f,
	-0.3f,-0.3f,-0.3f,
	0.3f,-0.3f,0.3f,

	-0.3f,-0.3f,-0.3f,
	0.3f,-0.3f,-0.3f,
	0.3f,-0.3f,0.3f,
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
	//밑면
	0.0f, 0.0f, 0.2f,
	-0.1f, 0.0f, -0.1f,
	0.1f, 0.0f, -0.1f,

	//뒷면
	-0.1f, 0.0f, -0.1f,
	0.1f, 0.0f, -0.1f,
	0.0f, 0.4f, 0.0f,

	//왼쪽면
	0.1f, 0.0f, -0.1f,
	0.0f, 0.0f, 0.2f,
	0.0f, 0.4f, 0.0f,

	//오른쪽면
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
	glutCreateWindow("Example_2_5(10번)");

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
	glEnable(GL_DEPTH_TEST);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	axes.transformations = glm::rotate(axes.transformations, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	axes.transformations = glm::rotate(axes.transformations, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	//좌표축 그리기

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	glm::mat4 Rz = glm::mat4(1.0f);
	//육면체 그리기

	Rz = glm::rotate(Rz, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rz));
	
	if (poly_state)
	{
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, hexa_vertex.size() / 3);
	}
	else
	{
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, tetra_vertex.size() / 3);
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
	else if (key == 'p')
	{
		poly_state = poly_state ? false : true;
	}
	else if (key == 'h')
	{
		culling_on = culling_on ? false : true;
		if (culling_on)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
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
	glBufferData(GL_ARRAY_BUFFER, axes.axes_color.size() * sizeof(GLfloat), axes.axes_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_vertex.size() * sizeof(GLfloat), axes.axes_vertex.data(), GL_STATIC_DRAW);
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
