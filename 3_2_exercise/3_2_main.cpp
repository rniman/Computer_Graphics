#define _USE_MATH_DEFINES

#include "axes.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid SpecialKeyEvent(int key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);
GLint check_mouse_point(const GLfloat& ox, const GLfloat& oy, const std::vector<GLfloat>& ver);
GLboolean isInside(const GLfloat& ox, const GLfloat& oy, const std::vector<GLfloat>& ver);


const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;
GLboolean poly_state = true;
GLboolean culling_on = false;
GLboolean fill_on = true;
GLboolean rotate_x = false;
GLboolean dir_plus_x = true;
GLboolean rotate_y = false;
GLboolean dir_plus_y = true;

GLuint vao[3];
GLuint vbo_axes[2];
GLuint vbo[2];
GLuint vbo_tetra[2];
unsigned int modelLocation;

axes_coordination axes;
glm::mat4 obj_transformations = glm::mat4(1.0f);


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
std::vector<GLfloat> pyramid_vertex =
{
	//밑면1
	0.3f, 0.0f, -0.3f,
	0.3f, 0.0f, 0.3f,
	-0.3f, 0.0f, -0.3f,

	//밑면2
	-0.3f, 0.0f, 0.3f,
	-0.3f, 0.0f, -0.3f,
	0.3f, 0.0f, 0.3f,

	//앞면
	0.0f,0.4f,0.0f,
	-0.3f, 0.0f, 0.3f,
	0.3f, 0.0f, 0.3f,
	
	//왼쪽면
	0.0f,0.4f,0.0f,
	0.3f, 0.0f, 0.3f,
	0.3f, 0.0f, -0.3f,
	
	//뒷면
	0.0f,0.4f,0.0f,
	0.3f, 0.0f, -0.3f,
	-0.3f, 0.0f, -0.3f,
	
	//오른쪽면	
	0.0f,0.4f,0.0f,
	-0.3f, 0.0f, -0.3f,
	-0.3f, 0.0f, 0.3f,
};
std::vector<GLfloat> pyramid_color =
{
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	
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
	0.0f,0.0f,1.0f,

	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 200);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_2(13번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutSpecialFunc(SpecialKeyEvent);

	//glEnable(GL_DEPTH_TEST);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	axes.transformations = glm::rotate(axes.transformations, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	axes.transformations = glm::rotate(axes.transformations, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	obj_transformations = glm::rotate(obj_transformations, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	obj_transformations = glm::rotate(obj_transformations, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));

	glutMainLoop();
}


GLvoid drawScene()
{

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glDisable(GL_DEPTH_TEST);
	//좌표축 그리기

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	glEnable(GL_DEPTH_TEST);

	//육면체 그리기

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(obj_transformations));
	
	if (poly_state)
	{
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, hexa_vertex.size() / 3);
	}
	else
	{
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, pyramid_vertex.size() / 3);
	}
	


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid TimeEvent(int value)
{
	if (rotate_x)
	{
		if(dir_plus_x)
			obj_transformations = glm::rotate(obj_transformations, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		else
			obj_transformations = glm::rotate(obj_transformations, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (rotate_y)
	{
		if (dir_plus_y)
			obj_transformations = glm::rotate(obj_transformations, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		else
			obj_transformations = glm::rotate(obj_transformations, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
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
	else if (key == 'x')
	{
		rotate_x = true;
		dir_plus_x = true;
	}
	else if (key == 'X')
	{
		rotate_x = true;
		dir_plus_x = false;
	}
	else if (key == 'y')
	{
		rotate_y = true;
		dir_plus_y = true;
	}
	else if (key == 'Y')
	{
		rotate_y = true;
		dir_plus_y = false;
	}
	else if (key == 'w')
	{
		fill_on = fill_on ? false : true;
		if (fill_on)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == 's')
	{
		rotate_x = false;
		rotate_y = false;
		obj_transformations = glm::mat4(1.0f);
		obj_transformations = glm::rotate(obj_transformations, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		obj_transformations = glm::rotate(obj_transformations, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
	}



	//glutPostRedisplay();
}

GLvoid SpecialKeyEvent(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
	{
		obj_transformations = glm::translate(obj_transformations, glm::vec3(0.01f, 0.0f, 0.0f));
	}
	else if (key == GLUT_KEY_LEFT)
	{
		obj_transformations = glm::translate(obj_transformations, glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	else if (key == GLUT_KEY_UP)
	{
		obj_transformations = glm::translate(obj_transformations, glm::vec3(0.0f, 0.01f, 0.0f));
	}
	else if (key == GLUT_KEY_DOWN)
	{
		obj_transformations = glm::translate(obj_transformations, glm::vec3(0.0f, -0.01f, 0.0f));
	}
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
	glBufferData(GL_ARRAY_BUFFER, pyramid_color.size() * sizeof(GLfloat), pyramid_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_tetra[0]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_vertex.size() * sizeof(GLfloat), pyramid_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}