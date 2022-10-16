#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;
GLboolean culling_on = true;
GLboolean fill_on = false;
GLboolean revolution = false;
GLboolean revolution_dir = true;
GLboolean pause = false;
GLboolean key_c = false;


GLint degree_rev = 0;

GLboolean left_obj_rotete_x = false;
GLboolean left_obj_rotete_y = false;
GLboolean left_dir_x = true;
GLboolean left_dir_y = true;
GLint degree_left = 0;
GLint degree_left_y = 0;



GLboolean right_obj_rotete_x = false;
GLboolean right_obj_rotete_y = false;
GLboolean right_dir_x = true;
GLboolean right_dir_y = true;
GLint degree_right = 0;
GLint degree_right_y = 0;



GLuint vao[5];
GLuint vbo_axes[2];
GLuint vbo[2];
GLuint vbo_sphere[2];
GLuint vbo_cylinder[2];
GLuint vbo_cone[2];

unsigned int modelLocation;
unsigned int viewLocation;

glm::mat4 camera;

axes_coordination axes;
glm::mat4 hexa_trasformation = glm::mat4(1.0f);
glm::mat4 sphere_trasformation = glm::mat4(1.0f);

objRead sphere;
GLint sphere_size = sphere.loadObj_normalize_center("sphere.obj");

objRead cylinder;
GLint cylinder_size = cylinder.loadObj_normalize_center("cylinder.obj");

objRead cone;
GLint cone_size = cone.loadObj_normalize_center("cone.obj");


std::vector<GLfloat> hexa_vertex =
{
	//뒷면
	-0.1f,0.1f,0.1f,
	-0.1f,-0.1f,0.1f,
	0.1f,0.1f,0.1f,

	-0.1f,-0.1f,0.1f,
	0.1f,-0.1f,0.1f,
	0.1f,0.1f,0.1f,

	//왼쪽면
	0.1f,0.1f,0.1f,
	0.1f,-0.1f,0.1f,
	0.1f,0.1f,-0.1f,

	0.1f,-0.1f,0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f,0.1f,-0.1f,

	//앞면
	0.1f,0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	-0.1f,0.1f,-0.1f,

	0.1f,-0.1f,-0.1f,
	-0.1f,-0.1f,-0.1f,
	-0.1f,0.1f,-0.1f,

	//오른쪽면
	-0.1f,0.1f,-0.1f,
	-0.1f,-0.1f,-0.1f,
	-0.1f,0.1f,0.1f,

	-0.1f,-0.1f,-0.1f,
	-0.1f,-0.1f,0.1f,
	-0.1f,0.1f,0.1f,

	//윗면
	-0.1f,0.1f,-0.1f,
	-0.1f,0.1f,0.1f,
	0.1f,0.1f,-0.1f,

	-0.1f,0.1f,0.1f,
	0.1f,0.1f,0.1f,
	0.1f,0.1f,-0.1f,

	//아랫면
	-0.1f,-0.1f,0.1f,
	-0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,0.1f,

	-0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,0.1f,
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

std::vector<GLfloat> obj_color =
{
	0.0f, 0.0f, 0.0f
};

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 200);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_3(14번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");

	camera = glm::lookAt(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	//camera = glm::lookAt(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.0f, 0.2f, -0.0f));

	
	hexa_trasformation = glm::mat4(1.0f);
	hexa_trasformation = glm::translate(hexa_trasformation, glm::vec3(0.0f, 0.0f, -0.5f));


	sphere_trasformation = glm::mat4(1.0f);
	sphere_trasformation = glm::translate(sphere_trasformation, glm::vec3(0.0f, 0.0f, 0.5f));

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));


	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	//좌표축 그리기

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	if (!key_c)
	{
		//육면체 그리기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(hexa_trasformation));
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, hexa_vertex.size() / 3);

		//구 그리기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sphere_trasformation));
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, sphere_size);
	}
	else
	{
		//실린더 그리기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(hexa_trasformation));
		glBindVertexArray(vao[3]);
		glDrawArrays(GL_TRIANGLES, 0, cylinder_size);

		//콘 그리기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sphere_trasformation));
		glBindVertexArray(vao[4]);
		glDrawArrays(GL_TRIANGLES, 0, cone_size);
	}
	


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid TimeEvent(int value)
{
	if (pause)
		return;

	hexa_trasformation = glm::mat4(1.0f);
	sphere_trasformation = glm::mat4(1.0f);

	hexa_trasformation = glm::rotate(hexa_trasformation, glm::radians((GLfloat)degree_rev), glm::vec3(0.0f, 1.0f, 0.0f));
	sphere_trasformation = glm::rotate(sphere_trasformation, glm::radians((GLfloat)degree_rev), glm::vec3(0.0f, 1.0f, 0.0f));
	if (revolution)
	{
		if (revolution_dir)
			degree_rev = (degree_rev + 10) % 360;
		else
			degree_rev = (degree_rev - 10) % 360;
	}

	hexa_trasformation = glm::translate(hexa_trasformation, glm::vec3(0.0f, 0.0f, -0.5f));
	sphere_trasformation = glm::translate(sphere_trasformation, glm::vec3(0.0f, 0.0f, 0.5f));

	if (left_obj_rotete_x)
	{
		if (left_dir_x)
			degree_left = (degree_left + 10) % 360;
		else
			degree_left = (degree_left - 10) % 360;
		hexa_trasformation = glm::rotate(hexa_trasformation, glm::radians((GLfloat)degree_left), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (left_obj_rotete_y)
	{
		if (left_dir_y)
			degree_left_y = (degree_left_y + 10) % 360;
		else
			degree_left_y = (degree_left_y - 10) % 360;
		hexa_trasformation = glm::rotate(hexa_trasformation, glm::radians((GLfloat)degree_left_y), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (right_obj_rotete_x)
	{
		if (right_dir_x)
			degree_right = (degree_right + 10) % 360;
		else
			degree_right = (degree_right - 10) % 360;
		sphere_trasformation = glm::rotate(sphere_trasformation, glm::radians((GLfloat)degree_right), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (right_obj_rotete_y)
	{
		if (right_dir_y)
			degree_right_y = (degree_right_y + 10) % 360;
		else
			degree_right_y = (degree_right_y - 10) % 360;
		sphere_trasformation = glm::rotate(sphere_trasformation, glm::radians((GLfloat)degree_right_y), glm::vec3(0.0f, 1.0f, 0.0f));
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
		pause = pause ? false : true;
		if(!pause)
			glutTimerFunc(100, TimeEvent, 0);
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
		left_obj_rotete_x = true;
		left_dir_x = true;
	}
	else if (key == 'X')
	{
		left_obj_rotete_x = true;
		left_dir_x = false;
	}
	else if (key == 'y')
	{
		left_obj_rotete_y = true;
		left_dir_y = true;
	}
	else if (key == 'Y')
	{
		left_obj_rotete_y = true;
		left_dir_y = false;
	}
	else if (key == 'a')
	{
		right_obj_rotete_x = true;
		right_dir_x = true;
	}
	else if (key == 'A')
	{
		right_obj_rotete_x = true;
		right_dir_x = false;
	}
	else if (key == 'b')
	{
		right_obj_rotete_y = true;
		right_dir_y = true;
	}
	else if (key == 'B')
	{
		right_obj_rotete_y = true;
		right_dir_y = false;
	}
	else if (key == 'w')
	{
		fill_on = fill_on ? false : true;
		if (fill_on)
			glPolygonMode(GL_FRONT, GL_FILL);
		else
			glPolygonMode(GL_FRONT, GL_LINE);
	}
	else if (key == 'r')
	{
		revolution =  true;
		revolution_dir = true; 
	}
	else if (key == 'R')
	{
		revolution = true;
		revolution_dir = false;
	}
	else if (key == 's')
	{
		revolution = false;
		revolution_dir = true;

		left_obj_rotete_x = false;
		left_obj_rotete_y = false;
		left_dir_x = true;
		left_dir_y = true;
		right_obj_rotete_x = false;
		right_obj_rotete_y = false;
		right_dir_x = true;
		right_dir_y = true;

		degree_left = 0;
		degree_right = 0;
		degree_rev = 0;
	}
	else if (key == 'c')
	{
		key_c = key_c ? false : true;
	}

}


void initBuffer()
{
	glGenVertexArrays(5, vao);
	glGenBuffers(2, vbo_axes);
	glGenBuffers(2, vbo);
	glGenBuffers(2, vbo_sphere);
	glGenBuffers(2, vbo_cylinder);
	glGenBuffers(2, vbo_cone);

	glBindVertexArray(vao[0]);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[1]);
	glBufferData(GL_ARRAY_BUFFER, obj_color.size() * sizeof(GLfloat), obj_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[0]);
	glBufferData(GL_ARRAY_BUFFER, sphere.outvertex.size() * sizeof(glm::vec3), sphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[3]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cylinder[1]);
	glBufferData(GL_ARRAY_BUFFER, obj_color.size() * sizeof(GLfloat), obj_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cylinder[0]);
	glBufferData(GL_ARRAY_BUFFER, cylinder.outvertex.size() * sizeof(glm::vec3), cylinder.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[4]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cone[1]);
	glBufferData(GL_ARRAY_BUFFER, obj_color.size() * sizeof(GLfloat), obj_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cone[0]);
	glBufferData(GL_ARRAY_BUFFER, cone.outvertex.size() * sizeof(glm::vec3), cone.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}