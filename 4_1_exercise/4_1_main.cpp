#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "cuboid.h"

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.5f, gColor = 0.5f, bColor = 0.5f;

GLboolean cuboid = true;

namespace keyState
{
	GLboolean h = true;
	GLboolean t = false;
	GLboolean f = false;
	GLboolean g = false;
	GLboolean o = false;
	GLboolean p = true;
	GLboolean y = false;
	GLboolean r = false;
	GLboolean m = true;
}

GLuint vao_light;
GLuint vbo_light[2];

GLuint vao_circle;
GLuint vbo_circle;

GLuint vao[12];
GLuint vbo_axes[2];
GLuint plane_vbo[11][2];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int objColorLocation;
unsigned int viewPosLocation;


glm::mat4 camera;
glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;
glm::mat4 cuboid_trasformation[6];
glm::mat4 pyramid_transformation[5];

GLfloat angle = 0.0f;
GLfloat tAngle = 0.0f;

GLfloat frontAngle = 0.0f;
GLboolean openFront = true;
GLfloat sideMove = 0.0f;
GLboolean openSide = true;

GLfloat pyramidAngle = 0.0f;
GLboolean openpyramid = true;

std::vector<GLfloat> light_vertex(108);
std::vector<GLfloat> light_normal(108);
glm::mat4 light_transformation = glm::mat4(1.0f);

std::vector<std::vector<GLfloat>> cuboid_plane =
{
	{
		//앞면 0
		-100.0f,100.0f,100.0f,
		-100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f,

		-100.0f,-100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f
	},

	{
		//오른면 1
		100.0f,100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		100.0f,-100.0f,100.0f,
		100.0f,-100.0f,-100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//뒷면 2
		100.0f,100.0f,-100.0f,
		100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f,

		100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f
	},
	{
		//왼면 3
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
std::vector<std::vector<GLfloat>> cuboid_normal =
{
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
	},
	{
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f
	},
	{
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f
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
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f
	}
};

std::vector<std::vector<GLfloat>> pyramid_plane =
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
		0.0f, 100.0f, 0.0f,
		-100.0f, 0.0f, -100.0f
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
std::vector<std::vector<GLfloat>> pyramid_normal =
{
	{
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f
	},
	{
		
	},
	{

	},
	{

	},
	{

	}
};

glm::vec3 cameraEye = glm::vec3(500.0f, 500.0f, 500.0f);
glm::vec3 originPos = glm::vec3(300.0f, 0.0f, 300.0f);
glm::vec3 lightPos = glm::vec3(300.0f, 0.0f, 300.0f);
GLfloat lightAngle = 0.0f;
GLboolean lightRotate = false;

std::vector<GLfloat> circle;
glm::mat4 circle_trans(1.0f);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_5(16번)");

	glewExperimental = GL_TRUE;
	glewInit();

	makeCuboid(light_vertex, 25, 25);
	setNormal(light_normal);

	for (int i = 1; i < 5; ++i)
	{
		glm::vec3 p0 = glm::vec3(pyramid_plane[i][0], pyramid_plane[i][1], pyramid_plane[i][2]);
		glm::vec3 p1 = glm::vec3(pyramid_plane[i][3], pyramid_plane[i][4], pyramid_plane[i][5]);
		glm::vec3 p2 = glm::vec3(pyramid_plane[i][6], pyramid_plane[i][7], pyramid_plane[i][8]);

		glm::vec3 temp = glm::cross(p1 - p0, p2 - p0);
		GLfloat normal_x = temp.x;
		GLfloat normal_y = temp.y;
		GLfloat normal_z = temp.z;

		for (int j = 0; j < 3; ++j) 
		{
			pyramid_normal[i].push_back(normal_x);
			pyramid_normal[i].push_back(normal_y);
			pyramid_normal[i].push_back(normal_z);
		}
	}

	for (float a = 0.0f; a < 360.0f; a += 10.0f)
	{
		circle.push_back(glm::sqrt(glm::pow(300.0f, 2) * 2) * glm::cos(glm::radians(a)));
		circle.push_back(0.0f);
		circle.push_back(glm::sqrt(glm::pow(300.0f, 2) * 2) * glm::sin(glm::radians(a)));
	}

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	projLocation = glGetUniformLocation(shaderID, "projectionTransform");
	lightPosLocation = glGetUniformLocation(shaderID, "lightPos"); //조명 위치
	lightColorLocation = glGetUniformLocation(shaderID, "lightColor"); //조명 색 설정
	objColorLocation = glGetUniformLocation(shaderID, "objectColor");  //주황색 반사
	viewPosLocation = glGetUniformLocation(shaderID, "cameraEye");     //--- viewPos 값 전달: 카메라 위치 

	camera = glm::lookAt(cameraEye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 2000.0f);


	for (int i = 0; i < 6; ++i)
		cuboid_trasformation[i] = glm::mat4(1.0f);
	for (int i = 0; i < 5; ++i)
		pyramid_transformation[i] = glm::mat4(1.0f);
	

	light_transformation = glm::mat4(1.0f);
	light_transformation = glm::translate(light_transformation, lightPos);

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);

	//광원체 위치 적용
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

	if(keyState::m)
		glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
	else
		glUniform3f(lightColorLocation, 0.0f, 0.0f, 0.0f);

	glUniform3f(viewPosLocation, cameraEye.x, cameraEye.y, cameraEye.z);

	//카메라 변환 적용
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
	
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	//좌표축 그리기
	glUniform3f(objColorLocation, 0.0f, 0.5f, 0.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformation));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(circle_trans));
	glBindVertexArray(vao_circle);
	glDrawArrays(GL_LINE_LOOP, 0, circle.size() / 3);

	if (cuboid)
	{
		//매질 설정
		glUniform3f(objColorLocation, 1.0f, 0.5f, 0.0f);
		//육면체 그리기
		for (int i = 0; i < 6; ++i)
		{

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cuboid_trasformation[i]));
			glBindVertexArray(vao[i + 1]);
			glDrawArrays(GL_TRIANGLES, 0, cuboid_plane[i].size() / 3);
		}
	}
	else
	{
		glUniform3f(objColorLocation, 1.0f, 0.5f, 1.0f);

		for (int i = 6; i < 11; ++i)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid_transformation[i - 6]));
			glBindVertexArray(vao[i + 1]);
			glDrawArrays(GL_TRIANGLES, 0, pyramid_plane[i - 6].size() / 3);
		}
	}

	glUniform3f(objColorLocation, 0.2f, 0.5f, 0.7f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(light_transformation));
	glBindVertexArray(vao_light);
	glDrawArrays(GL_TRIANGLES, 0, light_vertex.size() / 3);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	for (int i = 0; i < 6; ++i)
		cuboid_trasformation[i] = glm::mat4(1.0f);

	for (int i = 0; i < 5; ++i)
		pyramid_transformation[i] = glm::mat4(1.0f);

	if (keyState::t)
	{
		tAngle += 10.0f;
	}


	if (keyState::f)
	{
		if (openFront)
			frontAngle -= 5.0f;
		else
			frontAngle += 5.0f;

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
		if (openSide)
			sideMove += 10.0f;
		else
			sideMove -= 10.0f;

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

		if (openpyramid)
			pyramidAngle += 10.0f;
		else
			pyramidAngle -= 10.0f;


		if (pyramidAngle >= 270.0f && openpyramid)
		{
			keyState::o = false;
			openpyramid = false;

		}
		else if (pyramidAngle <= 0.0f && !openpyramid)
		{
			keyState::o = false;
			openpyramid = true;
		}
	}

	if (keyState::y)
	{
		angle += 5.0f;
	}

	cuboid_trasformation[4] = glm::rotate(cuboid_trasformation[4], glm::radians(tAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	cuboid_trasformation[0] = glm::translate(cuboid_trasformation[0], glm::vec3(0.0f, sideMove, 0.0f));
	cuboid_trasformation[2] = glm::translate(cuboid_trasformation[2], glm::vec3(0.0f, sideMove, 0.0f));


	for (int i = 0; i < 6; ++i)
		cuboid_trasformation[i] = glm::rotate(cuboid_trasformation[i], glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	cuboid_trasformation[1] = glm::translate(cuboid_trasformation[1], glm::vec3(100.0f, -100.0f, 0.0f));
	cuboid_trasformation[1] = glm::rotate(cuboid_trasformation[1], glm::radians(frontAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	cuboid_trasformation[1] = glm::translate(cuboid_trasformation[1], glm::vec3(-100.0f, 100.0f, 0.0f));


	for (int i = 0; i < 5; ++i)
		pyramid_transformation[i] = glm::rotate(pyramid_transformation[i], glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	pyramid_transformation[1] = glm::translate(pyramid_transformation[1], glm::vec3(100.0f, 0.0f, 0.0f));
	pyramid_transformation[1] = glm::rotate(pyramid_transformation[1], glm::radians(-pyramidAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	pyramid_transformation[1] = glm::translate(pyramid_transformation[1], glm::vec3(-100.0f, 0.0f, 0.0f));

	pyramid_transformation[2] = glm::translate(pyramid_transformation[2], glm::vec3(-100.0f, 0.0f, 0.0f));
	pyramid_transformation[2] = glm::rotate(pyramid_transformation[2], glm::radians(pyramidAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	pyramid_transformation[2] = glm::translate(pyramid_transformation[2], glm::vec3(100.0f, 0.0f, 0.0f));

	pyramid_transformation[3] = glm::translate(pyramid_transformation[3], glm::vec3(0.0f, 0.0f, -100.0f));
	pyramid_transformation[3] = glm::rotate(pyramid_transformation[3], glm::radians(-pyramidAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	pyramid_transformation[3] = glm::translate(pyramid_transformation[3], glm::vec3(0.0f, 0.0f, 100.0f));

	pyramid_transformation[4] = glm::translate(pyramid_transformation[4], glm::vec3(0.0f, 0.0f, 100.0f));
	pyramid_transformation[4] = glm::rotate(pyramid_transformation[4], glm::radians(pyramidAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	pyramid_transformation[4] = glm::translate(pyramid_transformation[4], glm::vec3(0.0f, 0.0f, -100.0f));

	if (keyState::r)
	{
		if (lightRotate)
			lightAngle += 5.0f;
		else
			lightAngle -= 5.0f;
	}
	glm::mat4 rotateMat(1.0f);
	glm::mat4 translateMat(1.0f);
	rotateMat = glm::rotate(rotateMat, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightPos = glm::vec3(rotateMat * glm::vec4(originPos, 1.0f));

	light_transformation = glm::mat4(1.0f);
	light_transformation = glm::rotate(light_transformation, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	light_transformation = glm::translate(light_transformation, originPos);

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
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
	else if (key == 'y')
	{
		keyState::y = keyState::y ? false : true;
	}
	else if (key == 't' && cuboid)
	{
		keyState::t = keyState::t ? false : true;
	}
	else if (key == 'f' && cuboid)
	{
		keyState::f = keyState::f ? false : true;
	}
	else if (key == 'g' && cuboid)
	{
		keyState::g = keyState::g ? false : true;
	}
	else if (key == 'c')
	{
		cuboid = cuboid ? false : true;

		for (int i = 0; i < 6; ++i)
			cuboid_trasformation[i] = glm::mat4(1.0f);
		for (int i = 0; i < 5; ++i)
			pyramid_transformation[i] = glm::mat4(1.0f);

		keyState::t = false;
		keyState::f = false;
		keyState::g = false;
		keyState::o = false;

		frontAngle = 0.0f;
		openFront = true;
		sideMove = 0.0f;
		openSide = true;

		pyramidAngle = 0.0f;
		openpyramid = true;
	}
	else if (key == 'o' && !cuboid)
	{
		keyState::o = keyState::o ? false : true;
	}
	else if (key == 'p')
	{
		keyState::p = keyState::p ? false : true;
		
		projection = glm::mat4(1.0f);
		if(!keyState::p)
			projection = glm::ortho(-700.0f, 700.0f, -700.0f, 700.0f, 0.1f, 2000.0f);
		else
			projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 2000.0f);
		
	}
	else if (key == 'r')
	{
		keyState::r = true;
		lightRotate = lightRotate ? false : true;
	}
	else if (key == 'z')
	{
		originPos.z += 10.0f;
		for (int i = 0; i < 36; ++i)
		{
			circle[3 * i] = glm::sqrt(glm::pow(originPos.x, 2) + glm::pow(originPos.z, 2)) * glm::cos(glm::radians(10.0f * i));
			circle[3 * i + 1] = 0.0f;
			circle[3 * i + 2] = glm::sqrt(glm::pow(originPos.x, 2) + glm::pow(originPos.z, 2)) * glm::sin(glm::radians(10.0f * i));
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo_circle);
		glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(GLfloat), circle.data(), GL_STATIC_DRAW);
	}
	else if (key == 'Z')
	{
		originPos.z -= 10.0f;
		for (int i = 0; i < 36; ++i)
		{
			circle[3 * i] = glm::sqrt(glm::pow(originPos.x, 2) + glm::pow(originPos.z, 2)) * glm::cos(glm::radians(10.0f * i));
			circle[3 * i + 1] = 0.0f;
			circle[3 * i + 2] = glm::sqrt(glm::pow(originPos.x, 2) + glm::pow(originPos.z, 2)) * glm::sin(glm::radians(10.0f * i));
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo_circle);
		glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(GLfloat), circle.data(), GL_STATIC_DRAW);
	}
	else if (key == 'm')
	{
		keyState::m = keyState::m ? false : true;
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


	glGenVertexArrays(1, &vao_light);
	glGenBuffers(2, vbo_light);

	glBindVertexArray(vao_light);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_light[1]);
	glBufferData(GL_ARRAY_BUFFER, light_normal.size() * sizeof(GLfloat), light_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_light[0]);
	glBufferData(GL_ARRAY_BUFFER, light_vertex.size() * sizeof(GLfloat), light_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &vao_circle);
	glGenBuffers(1, &vbo_circle);

	glBindVertexArray(vao_circle);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_circle);
	glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(GLfloat), circle.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 육면체 vbo vao
	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(vao[i + 1]);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][1]);
		glBufferData(GL_ARRAY_BUFFER, cuboid_normal[i].size() * sizeof(GLfloat), cuboid_normal[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][0]);
		glBufferData(GL_ARRAY_BUFFER, cuboid_plane[i].size() * sizeof(GLfloat), cuboid_plane[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	// 사면체
	for (int i = 6; i < 11; ++i)
	{
		glBindVertexArray(vao[i + 1]);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][1]);
		glBufferData(GL_ARRAY_BUFFER, pyramid_normal[i - 6].size() * sizeof(GLfloat), pyramid_normal[i - 6].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[i][0]);
		glBufferData(GL_ARRAY_BUFFER, pyramid_plane[i - 6].size() * sizeof(GLfloat), pyramid_plane[i - 6].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
}