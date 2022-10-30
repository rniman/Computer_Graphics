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
	GLboolean p = true;
	GLboolean m = true;
	GLboolean y = false;
}


GLuint vao[4];
GLuint vbo_axes[2];
GLuint vbo_sphere[3];
GLuint vbo_color[3];

GLuint vao_route[6];
GLuint vbo_route[6];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;

objRead bigSphere, standardSphere, smallSphere;
GLint bigSphereSize = bigSphere.loadObj_normalize_center("BigSphere.obj");
GLint standardSphereSize = standardSphere.loadObj_normalize_center("StandardSphere.obj");
GLint smallSphereSize = smallSphere.loadObj_normalize_center("SmallSphere.obj");


std::vector<std::vector<glm::vec3>> route
{
	{},{},{},{},{},{}
};
glm::mat4 transformationRoute[6];


std::vector<GLfloat>bigCol =
{
};
std::vector<GLfloat>standardCol =
{
};
std::vector<GLfloat> smallCol =
{
};

glm::mat4 transformationBigSphere = glm::mat4(1.0f);
glm::mat4 transformationStdSphere[3] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) };
glm::mat4 transformationSmallSphere[3] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) };

GLfloat stdAngle[3] = { 0.0f, 0.0f, 0.0f };
GLfloat smallAngle[3] = { 0.0f, 0.0f, 0.0f };
GLfloat allAngle = 0.0f;

glm::vec3 center[3];
glm::vec3 delta;

GLboolean startRotate = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_5(16번)");

	glewExperimental = GL_TRUE;
	glewInit();

	for (auto& e : bigSphere.outvertex)
	{
		bigCol.push_back(0.7f);
		bigCol.push_back(0.7f);
		bigCol.push_back(0.0f);

		standardCol.push_back(0.0f);
		standardCol.push_back(0.0f);
		standardCol.push_back(0.7f);

		smallCol.push_back(0.7f);
		smallCol.push_back(0.4f);
		smallCol.push_back(0.4f);
	}

	for (float i = 0; i < 360.0f; i += 10.0f)
	{
		route[0].push_back({ 140.0f * cos(glm::radians(i)), 0.0f, 140.0f * sin(glm::radians(i)) });
		
		route[1].push_back(
			{
			glm::cos(glm::radians(45.0f)) * glm::cos(glm::radians(i)) * 200.0f,
			glm::sin(glm::radians(45.0f)) * glm::cos(glm::radians(i)) * 200.0f, 
			glm::sin(glm::radians(i)) * 200.0f 
			});

		route[2].push_back(
			{
			glm::cos(glm::radians(-45.0f)) * glm::cos(glm::radians(i)) * 200.0f,
			glm::sin(glm::radians(-45.0f))* glm::cos(glm::radians(i)) * 200.0f,
			glm::sin(glm::radians(i)) * 200.0f
			});

		route[3].push_back({ cos(glm::radians(i)) * 50.0f, 0.0f, sin(glm::radians(i)) * 50.0f });

		route[4].push_back({ cos(glm::radians(i)) * 50.0f, 0.0f, sin(glm::radians(i)) * 50.0f });

		route[5].push_back({ cos(glm::radians(i)) * 50.0f, 0.0f, sin(glm::radians(i)) * 50.0f });

		//center[2].x = glm::cos(glm::radians(-45.0f)) * glm::cos(glm::radians(-stdAngle[2])) * 200.0f;
		//center[2].y = glm::sin(glm::radians(-45.0f)) * glm::cos(glm::radians(-stdAngle[2])) * 200.0f;
		//center[2].z = glm::sin(glm::radians(-stdAngle[2])) * 200.0f;
	}

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
	//camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 300.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
	//projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 400.0f);
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
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	//경로 그리기
	for (int i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformationRoute[i]));
		glBindVertexArray(vao_route[i]);
		glDrawArrays(GL_LINE_LOOP, 0, route[i].size());
	}

	//중심원(큰원)
	glBindVertexArray(vao[1]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformationBigSphere));
	glDrawArrays(GL_TRIANGLES, 0, bigSphereSize);

	//중간원
	glBindVertexArray(vao[2]);
	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformationStdSphere[i]));
		glDrawArrays(GL_TRIANGLES, 0, standardSphereSize);
	}
	
	//작은원
	glBindVertexArray(vao[3]);
	for (int i = 0; i < 3; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformationSmallSphere[i]));
		glDrawArrays(GL_TRIANGLES, 0, smallSphereSize);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	for(int i=0 ; i < 6 ;++i)
		transformationRoute[i] = glm::mat4(1.0f);

	transformationBigSphere = glm::mat4(1.0f);

	transformationStdSphere[0] = glm::mat4(1.0f);
	transformationStdSphere[1] = glm::mat4(1.0f);
	transformationStdSphere[2] = glm::mat4(1.0f);

	transformationSmallSphere[0] = glm::mat4(1.0f);
	transformationSmallSphere[1] = glm::mat4(1.0f);
	transformationSmallSphere[2] = glm::mat4(1.0f);


	if (keyState::y && startRotate)
		allAngle += 10.0f;
	else if(startRotate && !keyState::y)
		allAngle -= 10.0f;

	stdAngle[0] = stdAngle[0] + 8.0f;
	stdAngle[1] = stdAngle[1] + 10.0f;
	stdAngle[2] = stdAngle[2] + 5.0f;

	smallAngle[0] = smallAngle[0] + 17.0f;
	smallAngle[1] = smallAngle[1] + 17.0f;
	smallAngle[2] = smallAngle[2] + 17.0f;




	if (startRotate)
	{
		transformationBigSphere = glm::rotate(transformationBigSphere, glm::radians(allAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 3; ++i)
		{
			transformationStdSphere[i] = glm::rotate(transformationStdSphere[i], glm::radians(allAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			transformationSmallSphere[i] = glm::rotate(transformationSmallSphere[i], glm::radians(allAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		for (int i = 0; i < 6; ++i)
		{
			transformationRoute[i] = glm::rotate(transformationRoute[i], glm::radians(allAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}


	transformationBigSphere = glm::translate(transformationBigSphere, delta);
	for (int i = 0; i < 3; ++i)
	{
		transformationStdSphere[i] = glm::translate(transformationStdSphere[i], delta);
		transformationSmallSphere[i] = glm::translate(transformationSmallSphere[i], delta);
	}

	for (int i = 0; i < 6; ++i)
	{
		transformationRoute[i] = glm::translate(transformationRoute[i], delta);
	}

	transformationStdSphere[0] = glm::rotate(transformationStdSphere[0], glm::radians(stdAngle[0]), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationStdSphere[0] = glm::translate(transformationStdSphere[0], glm::vec3(0.0f, 0.0f, 140.0f));

	transformationStdSphere[1] = glm::rotate(transformationStdSphere[1], glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transformationStdSphere[1] = glm::rotate(transformationStdSphere[1], glm::radians(stdAngle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationStdSphere[1] = glm::translate(transformationStdSphere[1], glm::vec3(200.0f, 0.0f, 0.0f));

	transformationStdSphere[2] = glm::rotate(transformationStdSphere[2], glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transformationStdSphere[2] = glm::rotate(transformationStdSphere[2], glm::radians(stdAngle[2]), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationStdSphere[2] = glm::translate(transformationStdSphere[2], glm::vec3(200.0f, 0.0f, 0.0f));


	center[0].x = glm::sin(glm::radians(stdAngle[0])) * 140.0f;
	center[0].y = 0.0f;
	center[0].z = glm::cos(glm::radians(stdAngle[0])) * 140.0f;

	transformationSmallSphere[0] = glm::translate(transformationSmallSphere[0], center[0]);
	transformationSmallSphere[0] = glm::rotate(transformationSmallSphere[0], glm::radians(smallAngle[0]), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationSmallSphere[0] = glm::translate(transformationSmallSphere[0], glm::vec3(50.0f, 0.0f, 0.0f));
	
	center[1].x = glm::cos(glm::radians(45.0f)) * glm::cos(glm::radians(-stdAngle[1])) * 200.0f;
	center[1].y = glm::sin(glm::radians(45.0f)) * glm::cos(glm::radians(-stdAngle[1])) * 200.0f;
	center[1].z = glm::sin(glm::radians(-stdAngle[1])) * 200.0f;

	transformationSmallSphere[1] = glm::translate(transformationSmallSphere[1], center[1]);
	transformationSmallSphere[1] = glm::rotate(transformationSmallSphere[1], glm::radians(smallAngle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationSmallSphere[1] = glm::translate(transformationSmallSphere[1], glm::vec3(0.0f, 0.0f, -50.0f));
	
	center[2].x = glm::cos(glm::radians(-45.0f)) * glm::cos(glm::radians(-stdAngle[2])) * 200.0f;
	center[2].y = glm::sin(glm::radians(-45.0f)) * glm::cos(glm::radians(-stdAngle[2])) * 200.0f;
	center[2].z = glm::sin(glm::radians(-stdAngle[2])) * 200.0f;

	transformationSmallSphere[2] = glm::translate(transformationSmallSphere[2], center[2]);
	transformationSmallSphere[2] = glm::rotate(transformationSmallSphere[2], glm::radians(smallAngle[2]), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationSmallSphere[2] = glm::translate(transformationSmallSphere[2], glm::vec3(0.0f, 0.0f, -50.0f));


	transformationRoute[3] = glm::translate(transformationRoute[3], center[0]);
	transformationRoute[4] = glm::translate(transformationRoute[4], center[1]);
	transformationRoute[5] = glm::translate(transformationRoute[5], center[2]);


	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'p')
	{
		keyState::p = keyState::p ? false : true;
		projection = glm::mat4(1.0f);
		
		if(keyState::p)
			projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
		else
			projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 600.0f);	
	}
	else if (key == 'm')
	{
		keyState::m = keyState::m ? false : true;

		if(keyState::m)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == 'w')
	{
		delta.y += 5.0f;
	}
	else if (key == 's')
	{
		delta.y -= 5.0f;
	}
	else if (key == 'a')
	{
		delta.x -= 5.0f;
	}
	else if (key == 'd')
	{
		delta.x += 5.0f;
	}
	else if (key == 'z')
	{
		delta.z += 5.0f;
	}
	else if (key == 'x')
	{
		delta.z -= 5.0f;
	}
	else if (key == 'y')
	{
		keyState::y = keyState::y ? false : true;
		startRotate = true;
	}
}


void initBuffer()
{
	glGenVertexArrays(6, vao_route);
	glGenBuffers(6, vbo_route);

	glGenVertexArrays(4, vao);
	glGenBuffers(2, vbo_axes);
	glGenBuffers(3, vbo_sphere);
	glGenBuffers(3, vbo_color);

	for (int i = 0; i < 6; ++i)
	{
		glBindVertexArray(vao_route[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_route[i]);
		glBufferData(GL_ARRAY_BUFFER, route[i].size() * sizeof(glm::vec3), route[i].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
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


	glBindVertexArray(vao[1]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[0]);
	glBufferData(GL_ARRAY_BUFFER, bigCol.size() * sizeof(GLfloat), bigCol.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[0]);
	glBufferData(GL_ARRAY_BUFFER, bigSphere.outvertex.size() * sizeof(glm::vec3), bigSphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[1]);
	glBufferData(GL_ARRAY_BUFFER, standardCol.size() * sizeof(GLfloat), standardCol.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[1]);
	glBufferData(GL_ARRAY_BUFFER, standardSphere.outvertex.size() * sizeof(glm::vec3), standardSphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[3]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[2]);
	glBufferData(GL_ARRAY_BUFFER, smallCol.size() * sizeof(GLfloat), smallCol.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[2]);
	glBufferData(GL_ARRAY_BUFFER, smallSphere.outvertex.size() * sizeof(glm::vec3), smallSphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}