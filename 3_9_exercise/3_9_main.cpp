#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "Object.h"
#include "robot.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;

namespace keyState
{
	GLboolean h = true;
	GLboolean t = false;
	GLboolean f = false;
	GLboolean g = false;
	GLboolean o = false;
	GLboolean p = true;
	GLboolean m = false;
	GLboolean y = false;
	GLboolean b = false;
	GLboolean a = false;
}


GLuint vao[1];
GLuint vbo_axes[2];

GLuint vao_stage[6];
GLuint vbo_stage[12];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::vec3 camera_eye = {200.0f, 100.0f, 200.0f};

glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;
std::vector<std::vector<GLfloat>> stage =
{
	{
		//�޸� 0
		-100.0f,100.0f,100.0f,
		-100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f,

		-100.0f,-100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f
	},

	{
		//�ո� 1
		100.0f,100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		100.0f,-100.0f,100.0f,
		100.0f,-100.0f,-100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//������ 2
		100.0f,100.0f,-100.0f,
		100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f,

		100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f
	},
	{
		//�޸� 3
		-100.0f,100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,100.0f,

		-100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,100.0f,
		-100.0f,100.0f,100.0f
	},
	{
		//���� 4
		-100.0f,100.0f,-100.0f,
		-100.0f,100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		-100.0f,100.0f,100.0f,
		100.0f,100.0f,100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//�Ʒ��� 5
		-100.0f,-100.0f,100.0f,
		-100.0f,-100.0f,-100.0f,
		100.0f,-100.0f,100.0f,

		-100.0f,-100.0f,-100.0f,
		100.0f,-100.0f,-100.0f,
		100.0f,-100.0f,100.0f
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

robot Myrobot;

GLfloat deltaFront = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_7(18��)");

	glewExperimental = GL_TRUE;
	glewInit();

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);
	
	//���̴� �о�ͼ� ���̴� ���α׷� �����

	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");


	//camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.0f, -0.1f));
	camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 300.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	projection = glm::mat4(1.0f);
	//������� �����̰� ������� ����X
	//projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 600.0f);
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

	for (int i = 0; i < 6; ++i)
	{
		modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(stageTrans[i]));
		glBindVertexArray(vao_stage[i]);
		glDrawArrays(GL_TRIANGLES, 0, stage[i].size() / 3);
	}

	Myrobot.Draw(modelLocation);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);
	if (keyState::o)
	{
		if(deltaFront < 200.0f)
			deltaFront += 10.0f;
	}
	else
	{
		if(deltaFront > 0.0f)
			deltaFront -= 10.0f;
	}

	stageTrans[0] = glm::translate(stageTrans[0], glm::vec3(0.0f, deltaFront, 0.0f));

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'o')
	{
		keyState::o = keyState::o ? false : true;
	}
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

	Myrobot.init();
}