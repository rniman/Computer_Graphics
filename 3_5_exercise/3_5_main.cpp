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


GLboolean hexa = true;

namespace keyState
{
	GLboolean h = true;
	GLboolean t = false;
	GLboolean f = false;
	GLboolean g = false;
	GLboolean o = false;
	GLboolean p = false;
}

GLuint vao[12];
GLuint vbo_axes[2];
GLuint plane_vbo[11][2];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;
glm::mat4 hexa_trasformation[6];
glm::mat4 tetra_transformation[5];

GLfloat frontAngle = 0.0f;
GLboolean openFront = true;
GLfloat sideMove = 0.0f;
GLboolean openSide = true;

GLfloat tetraAngle = 0.0f;
GLboolean openTetra = true;

std::vector<std::vector<GLfloat>> hexa_plane =
{
	{
		//왼면 0
		-100.0f,100.0f,100.0f,
		-100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f,

		-100.0f,-100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f
	},

	{
		//앞면 1
		100.0f,100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		100.0f,-100.0f,100.0f,
		100.0f,-100.0f,-100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//오른면 2
		100.0f,100.0f,-100.0f,
		100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f,

		100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f
	},
	{
		//뒷면 3
		-100.0f,100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,100.0f,

		-100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,100.0f,
		-100.0f,100.0f,100.0f
	},
	{
		//윗면 4
		-100.0f,100.0f,-100.0f,
		-100.0f,100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		-100.0f,100.0f,100.0f,
		100.0f,100.0f,100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//아랫면 5
		-100.0f,-100.0f,100.0f,
		-100.0f,-100.0f,-100.0f,
		100.0f,-100.0f,100.0f,

		-100.0f,-100.0f,-100.0f,
		100.0f,-100.0f,-100.0f,
		100.0f,-100.0f,100.0f
	}
};

std::vector<std::vector<GLfloat>> hexa_color =
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

std::vector<std::vector<GLfloat>> tetra_plane =
{
	
	{//밑면
		-100.0f, 0.0f, 100.0f,
		-100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, 100.0f,

		-100.0f, 0.0f,-100.0f,
		100.0f, 0.0f,-100.0f,
		100.0f, 0.0f,100.0f
	},
	{//앞면
		100.0f, 0.0f, 100.0f,
		100.0f, 0.0f, -100.0f,
		0.0f, 100.0f, 0.0f
	},
	{//뒷면
		-100.0f, 0.0f, 100.0f,
		-100.0f, 0.0f, -100.0f,
		0.0f, 100.0f, 0.0f
	},
	{//오른면
		100.0f, 0.0f, -100.0f,
		-100.0f, 0.0f, -100.0f,
		0.0f, 100.0f, 0.0f
	},
	{//왼면
		-100.0f, 0.0f, 100.0f,
		100.0f, 0.0f, 100.0f,
		0.0f, 100.0f, 0.0f
	}
};
std::vector<std::vector<GLfloat>> tetra_color =
{
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
		1.0f,0.0f,1.0f
	},
	{
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f
	},
	{
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f
	},
	{
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	}
};

int main(int argc, char** argv)
{
	
	for (auto& e : tetra_plane)
	{
		for (auto& a : e)
			std::cout << a << ' ';
		std::cout << std::endl;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_5(16번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");


	camera = glm::lookAt(glm::vec3(200.0f, 100.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.0f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	//camera = glm::lookAt(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.0f, 0.2f, -0.0f));

	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 50.0f, 600.0f);
	//projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 400.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));

	for (int i = 0; i < 6; ++i)
		hexa_trasformation[i] = glm::mat4(1.0f);
	for (int i = 0; i < 5; ++i)
		tetra_transformation[i] = glm::mat4(1.0f);
	

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
	
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	//좌표축 그리기

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);


	if (hexa)
	{
		//육면체 그리기
		for (int i = 0; i < 6; ++i)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(hexa_trasformation[i]));
			glBindVertexArray(vao[i + 1]);
			glDrawArrays(GL_TRIANGLES, 0, hexa_plane[i].size() / 3);
		}
	}
	else
	{
		for (int i = 6; i < 11; ++i)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tetra_transformation[i - 6]));
			glBindVertexArray(vao[i + 1]);
			glDrawArrays(GL_TRIANGLES, 0, tetra_plane[i - 6].size() / 3);
		}
	}
	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	if (keyState::t)
	{
		hexa_trasformation[4] = glm::rotate(hexa_trasformation[4], glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (keyState::f)
	{
		hexa_trasformation[1] = glm::mat4(1.0f);
		if (openFront)
			frontAngle -= 5.0f;
		else
			frontAngle += 5.0f;

		hexa_trasformation[1] = glm::translate(hexa_trasformation[1], glm::vec3(100.0f, -100.0f, 0.0f));
		hexa_trasformation[1] = glm::rotate(hexa_trasformation[1], glm::radians(frontAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		hexa_trasformation[1] = glm::translate(hexa_trasformation[1], glm::vec3(-100.0f, 100.0f, 0.0f));

		if (frontAngle <= -90.0f && openFront)
		{
			keyState::f = false;
			openFront = false;
		}
		else if (frontAngle >= 0.0f && !openFront)
		{
			keyState::f = false;
			openFront = true;
		}
	}

	if (keyState::g)
	{
		hexa_trasformation[0] = glm::mat4(1.0f);
		hexa_trasformation[2] = glm::mat4(1.0f);

		if (openSide)
			sideMove += 10.0f;
		else
			sideMove -= 10.0f;

		hexa_trasformation[0] = glm::translate(hexa_trasformation[0], glm::vec3(0.0f, sideMove, 0.0f));
		hexa_trasformation[2] = glm::translate(hexa_trasformation[2], glm::vec3(0.0f, sideMove, 0.0f));

		if (sideMove >= 200.0f && openSide)
		{
			keyState::g = false;
			openSide = false;
		}
		else if (sideMove <= 0.0f && !openSide)
		{
			keyState::g = false;
			openSide = true;
		}
	}

	if (keyState::o)
	{
		for (int i = 0; i < 4; ++i)
			tetra_transformation[i + 1] = glm::mat4(1.0f);

		if (openTetra)
			tetraAngle += 10.0f;
		else
			tetraAngle -= 10.0f;

		tetra_transformation[1] = glm::translate(tetra_transformation[1], glm::vec3(100.0f, 0.0f, 0.0f));
		tetra_transformation[1] = glm::rotate(tetra_transformation[1], glm::radians(-tetraAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		tetra_transformation[1] = glm::translate(tetra_transformation[1], glm::vec3(-100.0f, 0.0f, 0.0f));

		tetra_transformation[2] = glm::translate(tetra_transformation[2], glm::vec3(-100.0f, 0.0f, 0.0f));
		tetra_transformation[2] = glm::rotate(tetra_transformation[2], glm::radians(tetraAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		tetra_transformation[2] = glm::translate(tetra_transformation[2], glm::vec3(100.0f, 0.0f, 0.0f));

		tetra_transformation[3] = glm::translate(tetra_transformation[3], glm::vec3(0.0f, 0.0f, -100.0f));
		tetra_transformation[3] = glm::rotate(tetra_transformation[3], glm::radians(-tetraAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		tetra_transformation[3] = glm::translate(tetra_transformation[3], glm::vec3(0.0f, 0.0f, 100.0f));

		tetra_transformation[4] = glm::translate(tetra_transformation[4], glm::vec3(0.0f, 0.0f, 100.0f));
		tetra_transformation[4] = glm::rotate(tetra_transformation[4], glm::radians(tetraAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		tetra_transformation[4] = glm::translate(tetra_transformation[4], glm::vec3(0.0f, 0.0f, -100.0f));

		if (tetraAngle >= 270.0f && openTetra)
		{
			keyState::o = false;
			openTetra = false;
		}
		else if (tetraAngle <= 0.0f && !openTetra)
		{
			keyState::o = false;
			openTetra = true;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'h')
	{

		keyState::h = keyState::h ? false : true;
		if (keyState::h)
		{
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_CULL_FACE);
		}
	}
	else if (key == 't' && hexa)
	{
		keyState::t = keyState::t ? false : true;
	}
	else if (key == 'f' && hexa)
	{
		keyState::f = keyState::f ? false : true;
	}
	else if (key == 'g' && hexa)
	{
		keyState::g = keyState::g ? false : true;
	}
	else if (key == 'c')
	{
		hexa = hexa ? false : true;

		for (int i = 0; i < 6; ++i)
			hexa_trasformation[i] = glm::mat4(1.0f);
		for (int i = 0; i < 5; ++i)
			tetra_transformation[i] = glm::mat4(1.0f);

		keyState::t = false;
		keyState::f = false;
		keyState::g = false;
		keyState::o = false;

		frontAngle = 0.0f;
		openFront = true;
		sideMove = 0.0f;
		openSide = true;

		tetraAngle = 0.0f;
		openTetra = true;
	}
	else if (key == 'o' && !hexa)
	{
		keyState::o = keyState::o ? false : true;
	}
	else if (key == 'p')
	{
		keyState::p = keyState::p ? false : true;
		
		projection = glm::mat4(1.0f);
		if(keyState::p)
			projection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 50.0f, 600.0f);
		else
		{
			projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 600.0f);
			projection = glm::translate(projection, glm::vec3(0.0, 0.0, -100.0)); //-> 어떤원리인지 모르겠다
		}
	}
}


void initBuffer()
{
	glGenVertexArrays(12, vao);
	glGenBuffers(2, vbo_axes);
	for (int i = 0; i < 11; ++i)
	{
		glGenBuffers(2, plane_vbo[i]);
	}

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[1]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_color.size() * sizeof(GLfloat), axes.axes_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_axes[0]);
	glBufferData(GL_ARRAY_BUFFER, axes.axes_vertex.size() * sizeof(GLfloat), axes.axes_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 육면체 vbo vao
	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(vao[i + 1]);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][1]);
		glBufferData(GL_ARRAY_BUFFER, hexa_color[i].size() * sizeof(GLfloat), hexa_color[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][0]);
		glBufferData(GL_ARRAY_BUFFER, hexa_plane[i].size() * sizeof(GLfloat), hexa_plane[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	// 사면체
	for (int i = 6; i < 11; ++i)
	{
		glBindVertexArray(vao[i + 1]);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][1]);
		glBufferData(GL_ARRAY_BUFFER, tetra_color[i - 6].size() * sizeof(GLfloat), tetra_color[i - 6].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][0]);
		glBufferData(GL_ARRAY_BUFFER, tetra_plane[i - 6].size() * sizeof(GLfloat), tetra_plane[i - 6].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
}