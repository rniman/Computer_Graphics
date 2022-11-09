#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "block.h"
#include "robot.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid KeyUpEvent(unsigned char key, int x, int y);
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
		//왼면 0
		-200.0f,200.0f,200.0f,
		-200.0f,-200.0f,200.0f,
		200.0f,200.0f,200.0f,

		-200.0f,-200.0f,200.0f,
		200.0f,-200.0f,200.0f,
		200.0f,200.0f,200.0f
	},

	{
		//앞면 1
		200.0f,200.0f,200.0f,
		200.0f,-200.0f,200.0f,
		200.0f,200.0f,-200.0f,

		200.0f,-200.0f,200.0f,
		200.0f,-200.0f,-200.0f,
		200.0f,200.0f,-200.0f
	},
	{
		//오른면 2
		200.0f,200.0f,-200.0f,
		200.0f,-200.0f,-200.0f,
		-200.0f,200.0f,-200.0f,

		200.0f,-200.0f,-200.0f,
		-200.0f,-200.0f,-200.0f,
		-200.0f,200.0f,-200.0f
	},
	{
		//뒷면 3
		-200.0f,200.0f,-200.0f,
		-200.0f,-200.0f,-200.0f,
		-200.0f,200.0f,200.0f,

		-200.0f,-200.0f,-200.0f,
		-200.0f,-200.0f,200.0f,
		-200.0f,200.0f,200.0f
	},
	{
		//윗면 4
		-200.0f,200.0f,-200.0f,
		-200.0f,200.0f,200.0f,
		200.0f,200.0f,-200.0f,

		-200.0f,200.0f,200.0f,
		200.0f,200.0f,200.0f,
		200.0f,200.0f,-200.0f
	},
	{
		//아랫면 5
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

robot Myrobot;

block block01;

GLfloat deltaFront = 0.0f;


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_7(18번)");

	glewExperimental = GL_TRUE;
	glewInit();

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);
	
	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutKeyboardUpFunc(KeyUpEvent);

	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");


	//camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.0f, -0.1f));
	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	//projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 1000.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));

	block01.posModel();

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//랜더링 파이프라인에 세이더 불러오기
	
	glUseProgram(shaderID);

	//카메라 변환 적용
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	
	//좌표축 그리기
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

	block01.draw(modelLocation);

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

	
	Myrobot.move(block01);
	if(Myrobot.jumpState)
		Myrobot.jumpRobot(block01);

	
	Myrobot.initMatrix();

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);
	if (keyState::o)
	{
		if(deltaFront < 400.0f)
			deltaFront += 10.0f;
		if (deltaFront == 400.0f)
			glEnable(GL_CULL_FACE);
 
	}
	else
	{
		if(deltaFront > 0.0f)
			deltaFront -= 10.0f;
	}

	stageTrans[0] = glm::translate(stageTrans[0], glm::vec3(0.0f, deltaFront, 0.0f));

	Myrobot.posModel(Myrobot.pos);
	Myrobot.lookModel();
	Myrobot.initRobotModel();
	Myrobot.swingModel();

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
	else if (key == 'i')
	{
		camera_eye = glm::vec3(0.0f, 0.0f, 500.0f);
		cameraAngle = 0.0f;
		deltaFront = 0.0f;
		Myrobot.reset();
	}
	else if (key == 'w')
	{
		if (!keyState::w)
		{
			keyState::w = true;
			Myrobot.Xdir += 1;
			Myrobot.changeLook();
		}
	}
	else if (key == 's')
	{
		if (!keyState::s)
		{
			keyState::s = true;
			Myrobot.Xdir -= 1;
			Myrobot.changeLook();
		}
	}
	else if (key == 'a')
	{
		if (!keyState::a)
		{
			keyState::a = true;
			Myrobot.Zdir -= 1;
			Myrobot.changeLook();
		}
	}
	else if (key == 'd')
	{
		if (!keyState::d)
		{
			keyState::d = true;
			Myrobot.Zdir += 1;
			Myrobot.changeLook();
		}
	}
	else if (key == 'z')
	{
		camera_eye.z += 10.0f;
	}
	else if (key == 'Z')
	{
		camera_eye.z -= 10.0f;
	}
	else if (key == 'x')
	{
		camera_eye.x += 10.0f;
	}
	else if (key == 'X')
	{
		camera_eye.x -= 10.0f;
	}
	else if (key == 'y')
	{
		cameraAngle += 10.0f;
	}
	else if (key == 'Y')
	{
		cameraAngle -= 10.0f;
	}
	else if (key == 'j')
	{
		if (!Myrobot.jumpState)
			Myrobot.jumpState = true;
	}
}

GLvoid KeyUpEvent(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		if (keyState::w)
		{
			keyState::w = false;
			Myrobot.Xdir -= 1;
			if(Myrobot.Xdir != 0 || Myrobot.Zdir != 0)
				Myrobot.changeLook();
		}
	}
	else if (key == 's')
	{
		if (keyState::s)
		{
			keyState::s = false;
			Myrobot.Xdir += 1;
			if (Myrobot.Xdir != 0 || Myrobot.Zdir != 0)
				Myrobot.changeLook();
		}
	}
	else if (key == 'a')
	{
		if (keyState::a)
		{
			keyState::a = false;
			Myrobot.Zdir += 1;
			if (Myrobot.Xdir != 0 || Myrobot.Zdir != 0)
				Myrobot.changeLook();
		}
	}
	else if (key == 'd')
	{
		if (keyState::d)
		{
			keyState::d = false;
			Myrobot.Zdir -= 1;
			if (Myrobot.Xdir != 0 || Myrobot.Zdir != 0)
				Myrobot.changeLook();
		}
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

	block01.initBuffuer();
}