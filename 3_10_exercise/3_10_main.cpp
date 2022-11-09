#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "cuboid.h"
#include "sphere.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseMove(int x, int y);


void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.0f, gColor = 0.0f, bColor = 0.0f;


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

GLfloat cameraAngle = 0.0f;
GLfloat cameraUpAngle = 0.0f;
glm::mat4 camera;
glm::vec3 camera_eye = glm::vec3(0.0f,0.0f,500.0f);
GLfloat value_floor = -200.0f;


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
	0.8f,0.8f,0.8f,
	0.8f,0.8f,0.8f,
	0.8f,0.8f,0.8f,
	0.8f,0.8f,0.8f,
	0.8f,0.8f,0.8f,
	0.8f,0.8f,0.8f
	},
	{
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f
	},
	{
	0.7f,0.7f,0.7f,
	0.7f,0.7f,0.7f,
	0.7f,0.7f,0.7f,
	0.7f,0.7f,0.7f,
	0.7f,0.7f,0.7f,
	0.7f,0.7f,0.7f
	},
	{
	0.4f,0.4f,0.4f,
	0.4f,0.4f,0.4f,
	0.4f,0.4f,0.4f,
	0.4f,0.4f,0.4f,
	0.4f,0.4f,0.4f,
	0.4f,0.4f,0.4f
	},
	{
	0.9f,0.9f,0.9f,
	0.9f,0.9f,0.9f,
	0.9f,0.9f,0.9f,
	0.9f,0.9f,0.9f,
	0.9f,0.9f,0.9f,
	0.9f,0.9f,0.9f
	},
	{
	0.2f,0.2f,0.2f,
	0.2f,0.2f,0.2f,
	0.2f,0.2f,0.2f,
	0.2f,0.2f,0.2f,
	0.2f,0.2f,0.2f,
	0.2f,0.2f,0.2f
	}
};

glm::mat4 stageTrans[6];

GLuint vao_cuboid[3];
GLuint vbo_cuboid_vertex[3];
GLuint vbo_cuboid_color[3];

std::vector<GLfloat> cuboid[3];
std::vector<GLfloat> cuboid_color[3];
glm::mat4 cuboidTrans[3];

GLuint vao_sphere;
GLuint vbo_sphere_vertex;
GLuint vbo_sphere_color;

std::vector<GLfloat> sphere_color;
glm::mat4 sphereTrans[5];

//objRead sphere;
sphere moveSphere(1);

GLfloat deltaFront = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_10(21번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//sphere.loadObj_normalize_center("sphere.obj");

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);
	
	for (int i = 0; i < 3; ++i)
	{
		makeCuboid(cuboid[i], 10 * (i + 1), 10 * (i + 1));
		setCol(cuboid_color[i], 0.3f, 0.8, 0.9);
		cuboidTrans[i] = glm::mat4(1.0f);
		cuboidTrans[i] = glm::translate(cuboidTrans[i], glm::vec3(0.0f, 0.0f, 30.0f * i));
	}

	//for (int i = 0; i < sphere.outvertex.size(); ++i)
	//{
	//	sphere_color.push_back(0.0f);
	//	sphere_color.push_back(0.7f);
	//	sphere_color.push_back(1.0f);
	//}

	for (int i = 0; i < 5; ++i)
		sphereTrans[i] = glm::mat4(1.0f);

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();
	moveSphere.init();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);

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
	//근평면은 포함이고 원평면은 포함X
	//projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 1000.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));

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

	glFrontFace(GL_CW);
	for (int i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(stageTrans[i]));
		glBindVertexArray(vao_stage[i]);
		glDrawArrays(GL_TRIANGLES, 0, stage[i].size() / 3);
	}
	glFrontFace(GL_CCW);

	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cuboidTrans[i]));
		glBindVertexArray(vao_cuboid[i]);
		glDrawArrays(GL_TRIANGLES, 0, cuboid[i].size() / 3);
	}

	//for (int i = 0; i < 5; ++i)
	//{
	//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sphereTrans[i]));
	//	glBindVertexArray(vao_sphere);
	//	glDrawArrays(GL_TRIANGLES, 0, sphere.outvertex.size());
	//}

	//for (int i = 0; i < 5; ++i)
	//{
	//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moveSphere.transformation[i]));
	//	glBindVertexArray(moveSphere.vao_sphere);
	//	glDrawArrays(GL_TRIANGLES, 0, moveSphere.object.outvertex.size());
	//}

	moveSphere.draw(modelLocation);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f));
	camera = glm::rotate(camera, glm::radians(cameraUpAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < 6; ++i)
		stageTrans[i] = glm::mat4(1.0f);

	stageTrans[0] = glm::translate(stageTrans[0], glm::vec3(0.0f, deltaFront, 0.0f));

	moveSphere.move();
	moveSphere.clamp(400.0f);
	moveSphere.model();


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
	else if (key == 'z')
	{
		camera_eye.z += 10.0f;
	}
	else if (key == 'Z')
	{
		camera_eye.z -= 10.0f;
	}
	else if (key == 'b')
	{
		if(moveSphere.pos.size() < 5)
			moveSphere.addSphere();
	}
}

GLboolean left_down = false;
GLfloat old_x, old_y;

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		left_down = true;
		old_x = x;
		old_y = y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		left_down = false;
	}
}

GLvoid MouseMove(int x, int y)
{
	if (left_down)
	{
		cameraUpAngle += (x - old_x);
		old_x = x;
		old_y = y;
		std::cout << cameraUpAngle << std::endl;
		//value_floor = 
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

	//glGenVertexArrays(1, &vao_sphere);
	//glGenBuffers(1, &vbo_sphere_vertex);
	//glGenBuffers(1, &vbo_sphere_color);

	//glBindVertexArray(vao_sphere);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_color);
	//glBufferData(GL_ARRAY_BUFFER, sphere_color.size() * sizeof(GLfloat), sphere_color.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_vertex);
	//glBufferData(GL_ARRAY_BUFFER, sphere.outvertex.size() * sizeof(glm::vec3), sphere.outvertex.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(0);

}