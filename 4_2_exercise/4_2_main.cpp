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
GLuint vbo_normal[3];

GLuint vao_route[6];
GLuint vbo_route[6];

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

objRead bigSphere, standardSphere, smallSphere;
GLint bigSphereSize = bigSphere.loadObj_normalize_center("BigSphere.obj");
GLint standardSphereSize = standardSphere.loadObj_normalize_center("StandardSphere.obj");
GLint smallSphereSize = smallSphere.loadObj_normalize_center("SmallSphere.obj");


std::vector<std::vector<glm::vec3>> route
{
	{},{},{},{},{},{}
};
glm::mat4 transformationRoute[6];


glm::mat4 transformationBigSphere = glm::mat4(1.0f);
glm::mat4 transformationStdSphere[3] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) };
glm::mat4 transformationSmallSphere[3] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) };

GLfloat stdAngle[3] = { 0.0f, 0.0f, 0.0f };
GLfloat smallAngle[3] = { 0.0f, 0.0f, 0.0f };
GLfloat allAngle = 0.0f;

glm::vec3 center[3];
glm::vec3 delta;

GLboolean startRotate = false;

glm::vec3 lightPos = glm::vec3(200.0f, 0.0f, 200.0);
glm::vec3 origin_lightPos = glm::vec3(200.0f, 0.0f, 200.0);
glm::vec3 cameraEye = glm::vec3(400.0f, 400.0f, 400.0f);
GLfloat cameraAngle = 0.0f;

unsigned int vao_light;
unsigned int vbo_light[2];
std::vector<GLfloat> light_vertex;
std::vector<GLfloat> light_normal;
glm::mat4 light_trans = glm::mat4(1.0f);

glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
GLint light_color_state = 0;
GLfloat light_angle = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_6(17번)");

	glewExperimental = GL_TRUE;
	glewInit();

	makeCuboid(light_vertex, 25, 25);
	setNormal(light_normal);

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

	}

	//세이더 읽어와서 세이더 프로그램 만들기

	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
	//근평면은 포함이고 원평면은 포함X
	projection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 10.0f, 1000.0f);

	glutMainLoop();
}


GLvoid drawScene()
{

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(lightColorLocation, light_color.x, light_color.y, light_color.z);
	glUniform3f(viewPosLocation, cameraEye.x, cameraEye.y, cameraEye.z);

	//랜더링 파이프라인에 세이더 불러오기
	
	glUseProgram(shaderID);

	//카메라 변환 적용
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));

	//투영 변환 적용
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(objColorLocation, 0.0f, 0.0f, 0.0f);
	//좌표축 그리기
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformation));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);


	//경로 그리기
	for (int i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transformationRoute[i]));
		glBindVertexArray(vao_route[i]);
		glDrawArrays(GL_LINE_LOOP, 0, route[i].size());
	}
	glUniform3f(objColorLocation, 0.3f, 0.2f, 0.4f);


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

	glBindVertexArray(vao_light);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(light_trans));
	glUniform3f(objColorLocation, light_color.x, light_color.y, light_color.z);
	glDrawArrays(GL_TRIANGLES, 0, light_vertex.size() / 3);

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


	light_trans = glm::mat4(1.0f);
	lightPos = origin_lightPos + delta;
	//light_trans = glm::translate(light_trans, delta);
	light_trans = glm::rotate(light_trans, glm::radians(allAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	light_trans = glm::translate(light_trans, lightPos);
	light_trans = glm::translate(light_trans, -origin_lightPos);
	light_trans = glm::rotate(light_trans, glm::radians(light_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	light_trans = glm::translate(light_trans, origin_lightPos);

	lightPos = glm::vec3(light_trans * glm::vec4(origin_lightPos, 1.0f));
	//std::cout << lightPos.x << '\t' << lightPos.z << std::endl;
	
	camera = glm::mat4(1.0f);
	camera = glm::lookAt(cameraEye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));

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
			projection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 10.0f, 1000.0f);
		else
			projection = glm::perspective(glm::radians(90.0f), 1.0f, 10.0f, 1000.0f);	
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
	else if (key == 'c')
	{
		light_color_state = (light_color_state + 1) % 3;

		if (light_color_state == 0)
		{
			light_color = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else if (light_color_state == 1)
		{
			light_color = glm::vec3(0.6f, 0.5f, 0.0f);
		}
		else if (light_color_state == 2)
		{
			light_color = glm::vec3(0.3f, 0.8f, 0.2f);
		}
	}
	else if (key == 'r')
	{
		light_angle += 10.0f;
	}
	else if (key == 'R')
	{
		light_angle -= 10.0f;
	}
	else if (key == 'n')
	{
		cameraAngle += 10.0f;
	}
}


void initBuffer()
{
	glGenVertexArrays(6, vao_route);
	glGenBuffers(6, vbo_route);

	glGenVertexArrays(4, vao);
	glGenBuffers(2, vbo_axes);
	glGenBuffers(3, vbo_sphere);
	glGenBuffers(3, vbo_normal);

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
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[0]);
	glBufferData(GL_ARRAY_BUFFER, bigSphere.outnormal.size() * sizeof(glm::vec3), bigSphere.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	std::cout << bigSphere.outnormal.size();

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[0]);
	glBufferData(GL_ARRAY_BUFFER, bigSphere.outvertex.size() * sizeof(glm::vec3), bigSphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[1]);
	glBufferData(GL_ARRAY_BUFFER, standardSphere.outnormal.size() * sizeof(glm::vec3), standardSphere.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[1]);
	glBufferData(GL_ARRAY_BUFFER, standardSphere.outvertex.size() * sizeof(glm::vec3), standardSphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[3]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[2]);
	glBufferData(GL_ARRAY_BUFFER, smallSphere.outnormal.size() * sizeof(glm::vec3), smallSphere.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[2]);
	glBufferData(GL_ARRAY_BUFFER, smallSphere.outvertex.size() * sizeof(glm::vec3), smallSphere.outvertex.data(), GL_STATIC_DRAW);
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

}