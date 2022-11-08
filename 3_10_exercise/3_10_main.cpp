#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "cuboid.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid KeyUpEvent(unsigned char key, int x, int y);
GLvoid MouseClick();
GLvoid MouseMove();

void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;

namespace keyState
{
	GLboolean h = false;
	GLboolean t = false;
	GLboolean f = false;
	GLboolean g = false;
	GLboolean o = false;
	GLboolean p = false;
	GLboolean m = false;
	GLboolean y = false;
	GLboolean b = false;
	GLboolean a = false;
	GLboolean w = false;
	GLboolean d = false;
	GLboolean s = false;
}


GLuint vao[1];
GLuint vbo_axes[2];

GLuint vao_stage[6];
GLuint vbo_stage[12];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::vec3 camera_eye = glm::vec3(0.0f,0.0f,500.0f);
GLfloat cameraAngle = 0.0f;

glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;
std::vector<std::vector<GLfloat>> stage =
{
	{
		//�޸� 0
		-200.0f,200.0f,200.0f,
		-200.0f,-200.0f,200.0f,
		200.0f,200.0f,200.0f,

		-200.0f,-200.0f,200.0f,
		200.0f,-200.0f,200.0f,
		200.0f,200.0f,200.0f
	},

	{
		//�ո� 1
		200.0f,200.0f,200.0f,
		200.0f,-200.0f,200.0f,
		200.0f,200.0f,-200.0f,

		200.0f,-200.0f,200.0f,
		200.0f,-200.0f,-200.0f,
		200.0f,200.0f,-200.0f
	},
	{
		//������ 2
		200.0f,200.0f,-200.0f,
		200.0f,-200.0f,-200.0f,
		-200.0f,200.0f,-200.0f,

		200.0f,-200.0f,-200.0f,
		-200.0f,-200.0f,-200.0f,
		-200.0f,200.0f,-200.0f
	},
	{
		//�޸� 3
		-200.0f,200.0f,-200.0f,
		-200.0f,-200.0f,-200.0f,
		-200.0f,200.0f,200.0f,

		-200.0f,-200.0f,-200.0f,
		-200.0f,-200.0f,200.0f,
		-200.0f,200.0f,200.0f
	},
	{
		//���� 4
		-200.0f,200.0f,-200.0f,
		-200.0f,200.0f,200.0f,
		200.0f,200.0f,-200.0f,

		-200.0f,200.0f,200.0f,
		200.0f,200.0f,200.0f,
		200.0f,200.0f,-200.0f
	},
	{
		//�Ʒ��� 5
		-200.0f,-200.0f,200.0f,
		-200.0f,-200.0f,-200.0f,
		200.0f,-200.0f,200.0f,

		-200.0f,-200.0f,-200.0f,
		200.0f,-200.0f,-200.0f,
		200.0f,-200.0f,200.0f
	}
};

std::vector<std::vector<GLfloat>> stage_color =
{
	{
	1.0f,0.5f,0.0f,
	1.0f,0.5f,0.0f,
	1.0f,0.5f,0.0f,
	1.0f,0.5f,0.0f,
	1.0f,0.5f,0.0f,
	1.0f,0.5f,0.0f
	},
	{
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f
	},
	{
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f
	},
	{
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f
	},
	{
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f
	},
	{
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f
	}
};

glm::mat4 stageTrans[6];

GLuint vao_cuboid[3];
GLuint vbo_cuboid_vertex[3];
GLuint vbo_cuboid_color[3];

std::vector<GLfloat> cuboid[3];
std::vector<GLfloat> cuboid_color[3];
glm::mat4 cuboidTrans[3];


GLfloat deltaFront = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_10(21��)");

	glewExperimental = GL_TRUE;
	glewInit();

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);
	
	for (int i = 0; i < 3; ++i)
	{
		makeCuboid(cuboid[i], 10 * (i + 1), 10 * (i + 1));
		setCol(cuboid_color[i], 0.3f, 0.8, 0.9);
		cuboidTrans[i] = glm::mat4(1.0f);
		cuboidTrans[i] = glm::translate(cuboidTrans[i], glm::vec3(0.0f, 0.0f, 30.0f * i));
	}
	//���̴� �о�ͼ� ���̴� ���α׷� �����

	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutKeyboardUpFunc(KeyUpEvent);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");


	//camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.0f, -0.1f));
	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	projection = glm::mat4(1.0f);
	//������� �����̰� ������� ����X
	//projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 1000.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));


	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//������ ���������ο� ���̴� �ҷ�����
	
	glUseProgram(shaderID);

	//ī�޶� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));

	//���� ��ȯ ����
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	
	//��ǥ�� �׸���
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformation));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	glFrontFace(GL_CW);
	for (int i = 0; i < 6; ++i)
	{
		modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(stageTrans[i]));
		glBindVertexArray(vao_stage[i]);
		glDrawArrays(GL_TRIANGLES, 0, stage[i].size() / 3);
	}
	glFrontFace(GL_CCW);

	for (int i = 0; i < 3; ++i)
	{
		modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cuboidTrans[i]));
		glBindVertexArray(vao_cuboid[i]);
		glDrawArrays(GL_TRIANGLES, 0, cuboid[i].size() / 3);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);

	stageTrans[0] = glm::translate(stageTrans[0], glm::vec3(0.0f, deltaFront, 0.0f));

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'y')
	{
		cameraAngle += 10.0f;
	}
	else if (key == 'Y')
	{
		cameraAngle -= 10.0f;
	}

}

GLvoid KeyUpEvent(unsigned char key, int x, int y)
{

}

void initBuffer()
{
	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo_axes);

	glBindVertexArray(vao[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[1]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_color.size() * sizeof(GLfloat), axes.axes_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_vertex.size() * sizeof(GLfloat), axes.axes_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glGenVertexArrays(6, vao_stage);
	glGenBuffers(12, vbo_stage);

	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(vao_stage[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_stage[2*i+1]);
		glBufferData(GL_ARRAY_BUFFER, stage_color[i].size() * sizeof(GLfloat), stage_color[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_stage[2*i]);
		glBufferData(GL_ARRAY_BUFFER, stage[i].size() * sizeof(GLfloat), stage[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}


	glGenVertexArrays(3, vao_cuboid);
	glGenBuffers(3, vbo_cuboid_vertex);
	glGenBuffers(3, vbo_cuboid_color);

	for (int i = 0; i < 3; ++i)
	{

		glBindVertexArray(vao_cuboid[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cuboid_color[i]);
		glBufferData(GL_ARRAY_BUFFER, cuboid_color[i].size() * sizeof(GLfloat), cuboid_color[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cuboid_vertex[i]);
		glBufferData(GL_ARRAY_BUFFER, cuboid[i].size() * sizeof(GLfloat), cuboid[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	}

}