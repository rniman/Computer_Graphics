#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimeEvent(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
void initBuffer();

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);
GLvoid convert_WindowXY_OpenglXY(const int& x, const int& y, float& ox, float& oy);
GLvoid add_triangle(std::vector<glm::vec3>& pyramid_vertex,const int& step);

const GLint window_w = 600, window_h = 600;
GLfloat rColor = 0.4f, gColor = 0.4f, bColor = 0.4f;



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
	GLboolean e = false;
	GLboolean l = false;
	GLboolean s = false;
	GLboolean _1 = false;
	GLboolean _2 = false;
	GLboolean _3 = false;
	GLboolean _4 = false;
	GLboolean _5 = false;
}


unsigned int pyramid_vao;
unsigned int pyramid_vbo[2];
glm::mat4 pyramid_trans[4];

std::vector<glm::vec3> pyramid_vertex
{
	//앞면
	glm::vec3(0.0f, 141.4f, 0.0f),
	glm::vec3(-100.0f, 0.0f, 100.0f),
	glm::vec3(100.0f, 0.0f, 100.0f),

	////뒷
	//glm::vec3(0.0f, 141.4f, 0.0f),
	//glm::vec3(100.0f, 0.0f, -100.0f),
	//glm::vec3(-100.0f, 0.0f, -100.0f),

	////왼
	//glm::vec3(0.0f, 141.4f, 0.0f),
	//glm::vec3(-100.0f, 0.0f, -100.0f),
	//glm::vec3(-100.0f, 0.0f, 100.0f ),

	////오
	//glm::vec3(0.0f, 141.4f, 0.0f),
	//glm::vec3(100.0f, 0.0f, 100.0f),
	//glm::vec3(100.0f, 0.0f, -100.0f)
};

std::vector<glm::vec3> pyramid_normal
{
};

GLuint vao[1];
GLuint vbo_axes[2];


GLuint floor_vao;
GLuint floor_vbo[2];
std::vector<GLfloat> floor_vertex
{
	//밑면
	-400.0f, 0.0f, -400.0f,
	-400.0f, 0.0f, 400.0f,
	400.0f, 0.0f, -400.0f,

	-400.0f, 0.0f, 400.0f,
	400.0f, 0.0f, 400.0f,
	400.0f, 0.0f, -400.0f
};
std::vector<GLfloat> floor_normal
{
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;
unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int objColorLocation;
unsigned int viewPosLocation;
unsigned int ambientLocation;


glm::mat4 camera;
glm::vec3 camera_eye = {0.0f, 300.0f, 500.0f};
glm::vec3 camera_origin_eye = {0.0f, 300.0f, 500.0f};
GLfloat cameraAngle = 0.0f;
GLfloat cameraRotateAngle = 0.0f;

glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;

glm::vec3 lightPos = {300.0f, 200.0f, 0.0f};
glm::vec3 lightDelta = { 0.0f,0.0f,0.0f };
glm::vec3 originLightPos = { 300.0f, 200.0f, 0.0f };
glm::vec3 lightColor =glm::vec3(1.0f,1.0f,1.0f);
glm::mat4 lightTrans = glm::mat4(1.0f);

GLuint vao_light;
GLuint vbo_light[2];
std::vector<GLfloat> light_vertex;
std::vector<GLfloat> light_normal;
GLfloat light_angle = 0.0f;

objRead big;
objRead middle;
objRead small;

GLuint big_vao;
GLuint middle_vao;
GLuint small_vao;
GLuint big_vbo[2];
GLuint middle_vbo[2];
GLuint small_vbo[2];
glm::mat4 big_trans(1.0f);
glm::mat4 middle_trans(1.0f);
glm::mat4 small_trans(1.0f);
glm::vec3 big_pos = { 300.0f, 0.0f, 0.0f };
glm::vec3 middle_pos = { 80.0f, 80.0f, 80.0f };
glm::vec3 small_pos = { 0.0f, 300.0f, 0.0f };
GLint sphere_angle = 0.0f;

struct Snow
{
	glm::vec3 pos;
	glm::mat4 transformation = glm::mat4(1.0f);
	GLfloat speed;
	GLvoid set_transformation();
};
GLvoid Snow::set_transformation()
{
	transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
}

std::vector<Snow> snow;

int num = 0;
int now_step = 0;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_7(18번)");

	glewExperimental = GL_TRUE;
	glewInit();	

	pyramid_trans[0] = glm::mat4(1.0f);
	pyramid_trans[1] = glm::mat4(1.0f);
	pyramid_trans[2] = glm::mat4(1.0f);
	pyramid_trans[3] = glm::mat4(1.0f);


	add_triangle(pyramid_vertex, 0);

	{

		glm::vec3 p1 = pyramid_vertex[1] - pyramid_vertex[0];

		glm::vec3 p2 = pyramid_vertex[2] - pyramid_vertex[0];

		glm::vec3 temp;
		temp = glm::cross(p1, p2);

		for (int i = 0; i < pyramid_vertex.size() / 3; ++i)
		{
			pyramid_normal.push_back(temp);
			pyramid_normal.push_back(temp);
			pyramid_normal.push_back(temp);
		}
	}
	glBindVertexArray(pyramid_vao);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_normal.size() * sizeof(glm::vec3), pyramid_normal.data(), GL_STATIC_DRAW);

	pyramid_trans[1] = glm::rotate(pyramid_trans[1], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[2] = glm::rotate(pyramid_trans[2], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	pyramid_trans[3] = glm::rotate(pyramid_trans[3], glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	std::uniform_int_distribution<int> dis(-400, 400);
	std::uniform_int_distribution<int> dis_speed(1, 10);
	snow.resize(50);
	for (int i = 0; i < 50; ++i) 
	{
		snow[i].pos = glm::vec3(dis(gen), 500.0f, dis(gen));
		snow[i].transformation = glm::mat4(1.0f);
		snow[i].speed = (float)dis(gen) * 0.1f;
	}

	makeCuboid(light_vertex, 10, 10);
	setNormal(light_normal);

	big.loadObj_normalize_center("BigSphere.obj");
	middle.loadObj_normalize_center("StandardSphere.obj");
	small.loadObj_normalize_center("SmallSphere.obj");

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
	ambientLocation = glGetUniformLocation(shaderID, "ambientLight");

	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 1000.0f);

	glutMainLoop();
}


GLvoid drawScene()
{
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
	if (!keyState::l)
		glUniform1f(ambientLocation, 0.5f);
	else
		glUniform1f(ambientLocation, 1.0f);

	glUniform3f(viewPosLocation, camera_eye.x, camera_eye.y, camera_eye.z);
	glUniform3f(objColorLocation, 0.0f, 0.0f, 0.0f);

	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, window_w, window_h);

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

	glUniform1f(ambientLocation, 0.5f);
	glUniform3f(objColorLocation, 0.4, 0.4, 0.4);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glBindVertexArray(floor_vao);
	glDrawArrays(GL_TRIANGLES, 0, floor_vertex.size() / 3);

	//피라미드
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glBindVertexArray(pyramid_vao);
	for (int i = 0; i < 4; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid_trans[i]));
		
		int draw_num = 1;
		for (int i = 0; i < now_step; ++i)
			draw_num *= 3;


		if (now_step == 0)
		{
			glUniform3f(objColorLocation, 0.7, 0.4, 0.7);
			glDrawArrays(GL_TRIANGLES, 0, draw_num * 3);
		}
		else if (now_step == 1)
		{
			glUniform3f(objColorLocation, 0.7, 0.4, 0.7);
			glDrawArrays(GL_TRIANGLES, 3, draw_num * 3);
		}
		else if (now_step == 2)
		{
			glUniform3f(objColorLocation, 0.7, 0.4, 0.7);
			glDrawArrays(GL_TRIANGLES, 12, draw_num * 3);
		}
		else if (now_step == 3)
		{
			glUniform3f(objColorLocation, 0.7, 0.4, 0.7);
			glDrawArrays(GL_TRIANGLES, 39, draw_num * 3);
		}
		else if (now_step == 4)
		{
			glUniform3f(objColorLocation, 0.7, 0.4, 0.7);
			glDrawArrays(GL_TRIANGLES, 120, draw_num * 3);
		}
		else if (now_step == 5)
		{
			glUniform3f(objColorLocation, 0.7, 0.4, 0.7);
			glDrawArrays(GL_TRIANGLES, 363, draw_num * 3);
		}

		if (now_step != 0)
		{
			glm::mat4 temp = pyramid_trans[i];
			temp = glm::scale(temp, glm::vec3(0.99f, 0.99f, 0.99f));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(temp));
			glUniform3f(objColorLocation, 0.0, 0.0, 0.0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	}

	//발광체
	glUniform1f(ambientLocation, 1.0f);
	glBindVertexArray(vao_light);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lightTrans));
	glUniform3f(objColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glDrawArrays(GL_TRIANGLES, 0, light_vertex.size() / 3);

	//구
	glUniform3f(objColorLocation, 0.5, 0.0, 0.8);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(big_trans));
	glBindVertexArray(big_vao);
	glDrawArrays(GL_TRIANGLES, 0, big.outnormal.size());

	glUniform3f(objColorLocation, 0.0, 0.7, 0.8);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(middle_trans));
	glBindVertexArray(middle_vao);
	glDrawArrays(GL_TRIANGLES, 0, middle.outnormal.size());

	glUniform3f(objColorLocation, 0.6, 0.7, 0.2);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(small_trans));
	glBindVertexArray(small_vao);
	glDrawArrays(GL_TRIANGLES, 0, small.outnormal.size());

	if (keyState::s)
	{
		glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);
		modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glBindVertexArray(small_vao);
		for (int i = 0; i < 50; ++i)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(snow[i].transformation));
			glDrawArrays(GL_TRIANGLES, 0, small.outnormal.size());
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

	lightTrans = glm::mat4(1.0f);
	
	lightTrans = glm::rotate(lightTrans, glm::radians(light_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightTrans = glm::translate(lightTrans, originLightPos + lightDelta);
	
	lightPos = glm::vec3(lightTrans * glm::vec4(originLightPos, 1.0f));


	camera = glm::mat4(1.0f);
	camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	camera_eye = glm::vec3(camera * glm::vec4(camera_origin_eye, 1.0f));
	camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	big_trans = glm::mat4(1.0f);
	sphere_angle = (10 + sphere_angle) % 360;

	big_trans = glm::rotate(big_trans, glm::radians((float)sphere_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	big_trans = glm::translate(big_trans, big_pos);
	
	middle_trans = glm::mat4(1.0f);

	middle_trans = glm::rotate(middle_trans, glm::radians((float)sphere_angle), glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)));
	middle_trans = glm::translate(middle_trans, middle_pos);

	small_trans = glm::mat4(1.0f);

	small_trans = glm::rotate(small_trans, glm::radians((float)sphere_angle), glm::vec3(-1.0f, 0.0f, 0.0f));
	small_trans = glm::translate(small_trans, small_pos);

	if (keyState::s)
	{
		std::uniform_int_distribution<int> dis(-400, 400);
		for (int i = 0; i < 50; ++i)
		{
			snow[i].pos.y -= snow[i].speed;
			if (snow[i].pos.y < 0.0f)
				snow[i].pos = glm::vec3(dis(gen), 500.0f, dis(gen));
			snow[i].set_transformation();
		}
	}

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == '+')
	{
		if (lightColor.x >= 1.0f)
			return;
		lightColor.x += 0.1f;
		lightColor.y += 0.1f;
		lightColor.z += 0.1f;
	}
	else if (key == '-')
	{
		if (lightColor.x <= 0.0)
			return;
		lightColor.x -= 0.1f;
		lightColor.y -= 0.1f;
		lightColor.z -= 0.1f;
	}
	else if (key == 'i')
	{
		lightDelta -= glm::normalize(originLightPos);
	}
	else if (key == 'o')
	{
		lightDelta += glm::normalize(originLightPos);
	}
	else if (key == 'y')
	{
		cameraAngle += 10.0f;
	}
	else if (key == 'r')
	{
		light_angle += 10.0f;
	}
	else if (key == 'R')
	{
		light_angle -= 10.0f;
	}
	else if (key == 's')
	{
		keyState::s = keyState::s ? false : true;
	}
	else if (key == '0')
	{
		now_step = 0;
	}
	else if (key == '1')
	{
		now_step = 1;
	}
	else if (key == '2')
	{
		now_step = 2;
	}
	else if (key == '3')
	{
		now_step = 3;
	}
	else if (key == '4')
	{
		now_step = 4;
	}
	else if (key == '5')
	{
		now_step = 5;
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

	//pyramid
	glGenVertexArrays(1, &pyramid_vao);
	glGenBuffers(2, pyramid_vbo);

	glBindVertexArray(pyramid_vao);

	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_normal.size() * sizeof(glm::vec3), pyramid_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pyramid_vertex.size() * sizeof(glm::vec3), pyramid_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &floor_vao);
	glGenBuffers(2, floor_vbo);

	glBindVertexArray(floor_vao);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, floor_normal.size() * sizeof(GLfloat), floor_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, floor_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, floor_vertex.size() * sizeof(GLfloat), floor_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//구
	glGenVertexArrays(1, &big_vao);
	glGenBuffers(2, big_vbo);

	glBindVertexArray(big_vao);

	glBindBuffer(GL_ARRAY_BUFFER, big_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, big.outnormal.size() * sizeof(glm::vec3), big.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, big_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, big.outvertex.size() * sizeof(glm::vec3), big.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &middle_vao);
	glGenBuffers(2, middle_vbo);

	glBindVertexArray(middle_vao);

	glBindBuffer(GL_ARRAY_BUFFER, big_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, middle.outnormal.size() * sizeof(glm::vec3), middle.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, middle_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, middle.outvertex.size() * sizeof(glm::vec3), middle.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &small_vao);
	glGenBuffers(2, small_vbo);

	glBindVertexArray(small_vao);

	glBindBuffer(GL_ARRAY_BUFFER, small_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, small.outnormal.size() * sizeof(glm::vec3), small.outnormal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, small_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, small.outvertex.size() * sizeof(glm::vec3), small.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

int add_num = 0;

//0 - 1개 - 3
//1 - 4개 = 1 * 3 + 1
//2 - 13 = 3 * 3 + 4
//3 - 40 = 9 * 3 + 13
//4 - 121 = 27 * 3 + 40
//5 - 27 * 3 * 3 + 121
GLvoid add_triangle(std::vector<glm::vec3>& pyramid_vertex,const int& step)
{
	if (step == 5)
	{
		return;
	}

	int loop = 1;
	for (int i = 0; i < step; ++i)
		loop *= 3;

	for (int i = 0; i <  loop; ++i)
	{
		glm::vec3 p0 = glm::vec3(
			(pyramid_vertex[3 * num].x + pyramid_vertex[3 * num + 1].x) / 2,
			(pyramid_vertex[3 * num].y + pyramid_vertex[3 * num + 1].y) / 2,
			(pyramid_vertex[3 * num].z + pyramid_vertex[3 * num + 1].z) / 2);
		glm::vec3 p1 = glm::vec3(
			(pyramid_vertex[3 * num + 1].x + pyramid_vertex[3 * num + 2].x) / 2,
			(pyramid_vertex[3 * num + 1].y + pyramid_vertex[3 * num + 2].y) / 2,
			(pyramid_vertex[3 * num + 1].z + pyramid_vertex[3 * num + 2].z) / 2);
		glm::vec3 p2 = glm::vec3(
			(pyramid_vertex[3 * num + 2].x + pyramid_vertex[3 * num].x) / 2,
			(pyramid_vertex[3 * num + 2].y + pyramid_vertex[3 * num].y) / 2,
			(pyramid_vertex[3 * num + 2].z + pyramid_vertex[3 * num].z) / 2);

		pyramid_vertex.push_back(pyramid_vertex[3 * num]);
		pyramid_vertex.push_back(p0);
		pyramid_vertex.push_back(p2);

		pyramid_vertex.push_back(pyramid_vertex[3 * num + 1]);
		pyramid_vertex.push_back(p1);
		pyramid_vertex.push_back(p0);

		pyramid_vertex.push_back(pyramid_vertex[3 * num + 2]);
		pyramid_vertex.push_back(p2);
		pyramid_vertex.push_back(p1);
		num += 1;
	}
	add_triangle(pyramid_vertex, step + 1);
	
}