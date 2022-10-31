#define _USE_MATH_DEFINES
#include "axes.h"
#include "read_Obj.h"
#include "Object.h"

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
GLboolean Brotate = false;
GLboolean Mrotate = false;
GLboolean Trotate = false;
GLfloat delta_x = 0.0f;
GLfloat mAngle = 0.0f;
GLfloat tAngle = 0.0f;

GLuint vao[1];
GLuint vbo_axes[2];

GLuint vao_lowerBody;
GLuint vbo_lowerBody[2];
GLuint vao_upperBody;
GLuint vbo_upperBody[2];
GLuint vao_arm;
GLuint vbo_arm[2];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::vec3 camera_eye = {200.0f, 100.0f, 200.0f};
GLfloat cameraAngle = 0.0f;
GLfloat cameraRotateAngle = 0.0f;

glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;

object lowerBody;
object upperBody;
object arm;
glm::mat4 sec_arm = glm::mat4(1.0f);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_5(16번)");

	glewExperimental = GL_TRUE;
	glewInit();

	lowerBody.vertex =
	{
		//앞면
		
			-100.0f, 0.0f, 100.0f,
			100.0f, 50.0f, 100.0f,
			-100.0f, 50.0f, 100.0f,

			-100.0f, 0.0f, 100.0f,
			100.0f, 0.0f, 100.0f,
			100.0f, 50.0f, 100.0f
		,
		//오른면
		
			100.0f, 0.0f, 100.0f,
			100.0f, 50.0f, -100.0f,
			100.0f, 50.0f, 100.0f,

			100.0f, 0.0f, 100.0f,
			100.0f, 0.0f, -100.0f,
			100.0f, 50.0f, -100.0f
		,
		//뒷면
		
			100.0f, 0.0f, -100.0f,
			-100.0f, 0.0f, -100.0f,
			-100.0f, 50.0f, -100.0f,

			100.0f, 0.0f, -100.0f,
			-100.0f, 50.0f, -100.0f,
			100.0f, 50.0f, -100.0f
		,
		//왼면
		
			-100.0f, 0.0f, -100.0f,
			-100.0f, 0.0f, 100.0f,
			-100.0f, 50.0f, 100.0f,

			-100.0f, 0.0f, -100.0f,
			-100.0f, 50.0f, 100.0f,
			-100.0f, 50.0f, -100.0f
		,
		//윗면
		 
			100.0f, 50.0f, 100.0f,
			100.0f, 50.0f, -100.0f,
			-100.0f, 50.0f, 100.0f,

			-100.0f, 50.0f, 100.0f,
			100.0f, 50.0f, -100.0f,
			-100.0f, 50.0f, -100.0f
		,
		//뒷면
		
			100.0f, 0.0f, 100.0f,
			-100.0f, 0.0f, 100.0f,
			-100.0f, 0.0f, -100.0f,

			100.0f, 0.0f, 100.0f,
			-100.0f, 0.0f, -100.0f,
			100.0f, 0.0f, -100.0f
		
	};
	lowerBody.color =
	{
		
			0.0f,0.0f,0.5f,
			0.0f,0.0f,0.5f,
			0.0f,0.0f,0.5f,
			0.0f,0.0f,0.5f,
			0.0f,0.0f,0.5f,
			0.0f,0.0f,0.5f
		,
		
			0.5f,0.0f,0.5f,
			0.5f,0.0f,0.5f,
			0.5f,0.0f,0.5f,
			0.5f,0.0f,0.5f,
			0.5f,0.0f,0.5f,
			0.5f,0.0f,0.5f
		,
		
			0.0f,0.5f,0.5f,
			0.0f,0.5f,0.5f,
			0.0f,0.5f,0.5f,
			0.0f,0.5f,0.5f,
			0.0f,0.5f,0.5f,
			0.0f,0.5f,0.5f
		,
		
			0.5f,0.3f,0.3f,
			0.5f,0.3f,0.3f,
			0.5f,0.3f,0.3f,
			0.5f,0.3f,0.3f,
			0.5f,0.3f,0.3f,
			0.5f,0.3f,0.3f
		,
		
			0.7f,0.7f,0.7f,
			0.7f,0.7f,0.7f,
			0.7f,0.7f,0.7f,
			0.7f,0.7f,0.7f,
			0.7f,0.7f,0.7f,
			0.7f,0.7f,0.7f
		,
		
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f
		
	};
	upperBody.vertex =
	{
		//앞면

			-50.0f, 50.0f, 50.0f,
			50.0f, 75.0f, 50.0f,
			-50.0f, 75.0f, 50.0f,

			-50.0f, 50.0f, 50.0f,
			50.0f, 50.0f, 50.0f,
			50.0f, 75.0f, 50.0f
		,
		//오른면

			50.0f, 50.0f, 50.0f,
			50.0f, 75.0f, -50.0f,
			50.0f, 75.0f, 50.0f,

			50.0f, 50.0f, 50.0f,
			50.0f, 50.0f, -50.0f,
			50.0f, 75.0f, -50.0f
		,
		//뒷면

			50.0f, 50.0f, -50.0f,
			-50.0f, 50.0f, -50.0f,
			-50.0f, 75.0f, -50.0f,

			50.0f, 50.0f, -50.0f,
			-50.0f, 75.0f, -50.0f,
			50.0f, 75.0f, -50.0f
		,
		//왼면

			-50.0f, 50.0f, -50.0f,
			-50.0f, 50.0f, 50.0f,
			-50.0f, 75.0f, 50.0f,

			-50.0f, 50.0f, -50.0f,
			-50.0f, 75.0f, 50.0f,
			-50.0f, 75.0f, -50.0f
		,
		//윗면

			50.0f, 75.0f, 50.0f,
			50.0f, 75.0f, -50.0f,
			-50.0f, 75.0f, 50.0f,

			-50.0f, 75.0f, 50.0f,
			50.0f, 75.0f, -50.0f,
			-50.0f, 75.0f, -50.0f
		,
		//뒷면

			50.0f, 50.0f, 50.0f,
			-50.0f, 50.0f, 50.0f,
			-50.0f, 50.0f, -50.0f,

			50.0f, 50.0f, 50.0f,
			-50.0f, 50.0f, -50.0f,
			50.0f, 50.0f, -50.0f

	};
	upperBody.color =
	{

			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f
		,

			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f
		,

			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f
		,

			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f
		,

			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f
		,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f
	};

	arm.vertex =
	{
		//앞면

			-15.0f, 0.0f, 15.0f,
			15.0f, 70.0f, 15.0f,
			-15.0f, 70.0f, 15.0f,

			-15.0f, 0.0f, 15.0f,
			15.0f, 0.0f, 15.0f,
			15.0f, 70.0f, 15.0f
		,
		//오른면

			15.0f, 0.0f, 15.0f,
			15.0f, 70.0f, -15.0f,
			15.0f, 70.0f, 15.0f,

			15.0f, 0.0f, 15.0f,
			15.0f, 0.0f, -15.0f,
			15.0f, 70.0f, -15.0f
		,
		//뒷면

			15.0f, 0.0f, -15.0f,
			-15.0f, 0.0f, -15.0f,
			-15.0f, 70.0f, -15.0f,

			15.0f, 0.0f, -15.0f,
			-15.0f, 70.0f, -15.0f,
			15.0f, 70.0f, -15.0f
		,
		//왼면

			-15.0f, 0.0f, -15.0f,
			-15.0f, 0.0f, 15.0f,
			-15.0f, 70.0f, 15.0f,

			-15.0f, 0.0f, -15.0f,
			-15.0f, 70.0f, 15.0f,
			-15.0f, 70.0f, -15.0f
		,
		//윗면

			15.0f, 70.0f, 15.0f,
			15.0f, 70.0f, -15.0f,
			-15.0f, 70.0f, 15.0f,

			-15.0f, 70.0f, 15.0f,
			15.0f, 70.0f, -15.0f,
			-15.0f, 70.0f, -15.0f
		,
		//뒷면

			15.0f, 0.0f, 15.0f,
			-15.0f, 0.0f, 15.0f,
			-15.0f, 0.0f, -15.0f,

			15.0f, 0.0f, 15.0f,
			-15.0f, 0.0f, -15.0f,
			15.0f, 0.0f, -15.0f

	};


	arm.color =
	{

			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f
		,

			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f,
			1.0f,0.5f,0.0f
		,

			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f,
			0.0f,0.5f,1.0f
		,

			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f,
			0.5f,0.3f,0.5f
		,

			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f,
			0.4f,0.4f,0.4f
		,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f,
			0.5f,0.5f,0.0f
	};

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


	//camera = glm::lookAt(camera_eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	camera = glm::lookAt(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.0f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 300.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	projection = glm::mat4(1.0f);
	//근평면은 포함이고 원평면은 포함X
	projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 50.0f, 600.0f);
	//projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 600.0f);
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

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lowerBody.transformation));
	glBindVertexArray(vao_lowerBody);
	glDrawArrays(GL_TRIANGLES, 0, lowerBody.getSize());

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(upperBody.transformation));
	glBindVertexArray(vao_upperBody);
	glDrawArrays(GL_TRIANGLES, 0, upperBody.getSize());

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(arm.transformation));
	glBindVertexArray(vao_arm);
	glDrawArrays(GL_TRIANGLES, 0, arm.getSize());

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sec_arm));
	glDrawArrays(GL_TRIANGLES, 0, arm.getSize());

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{
	camera = glm::mat4(1.0f);
	camera = glm::lookAt(camera_eye, glm::vec3(camera_eye.x - 200.0f, 0.0f, camera_eye.z - 200.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (keyState::a)
		cameraRotateAngle += 10.0f;

	camera = glm::rotate(camera, glm::radians(cameraRotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	camera = glm::translate(camera, camera_eye);
	camera = glm::rotate(camera, glm::radians(cameraAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	camera = glm::translate(camera, -camera_eye);

	sec_arm = glm::mat4(1.0f);
	arm.transformation = glm::mat4(1.0f);
	lowerBody.transformation = glm::mat4(1.0f);
	upperBody.transformation = glm::mat4(1.0f);

	lowerBody.transformation = glm::translate(lowerBody.transformation, glm::vec3(delta_x, 0.0f, 0.0f));
	upperBody.transformation = glm::translate(upperBody.transformation, glm::vec3(delta_x, 0.0f, 0.0f));
	arm.transformation = glm::translate(arm.transformation, glm::vec3(delta_x, 0.0f, 0.0f));
	sec_arm = glm::translate(sec_arm, glm::vec3(delta_x, 0.0f, 0.0f));

	upperBody.transformation = glm::rotate(upperBody.transformation, glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	arm.transformation = glm::rotate(arm.transformation, glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	sec_arm = glm::rotate(sec_arm, glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	if (keyState::m)
	{
		if (Mrotate)
			mAngle += 10.0f;
		else
			mAngle -= 10.0f;
	}

	if (keyState::t)
	{
		if (Trotate)
			tAngle += 10.0f;
		else
			tAngle -= 10.0f;

		if (tAngle >= 90.0f && Trotate)
			Trotate = false;
		else if (tAngle <= -90.0f && !Trotate)
			Trotate = true;
	}

	arm.transformation = glm::translate(arm.transformation, glm::vec3(0.0f, 75.0f, 20.0f));
	sec_arm = glm::translate(sec_arm, glm::vec3(0.0f, 75.0f, -20.0f));
	
	arm.transformation = glm::rotate(arm.transformation, glm::radians(-tAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	sec_arm = glm::rotate(sec_arm, glm::radians(tAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'b')
	{
		delta_x += 10.0f;
	}
	else if (key == 'B')
	{
		delta_x -= 10.0f;
	}
	else if (key == 'm')
	{
		keyState::m = true;
		Mrotate = Mrotate ? false : true;
	}
	else if (key == 'M')
	{
		keyState::m = keyState::m ? false : true;
	}
	else if (key == 't')
	{
		keyState::t = true;
		Trotate = Trotate ? false : true;
	}
	else if (key == 'T')
	{
		keyState::t = keyState::t ? false : true;
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
	else if (key == 'r')
	{
		cameraRotateAngle += 10.0f;
	}
	else if (key == 'R')
	{
		cameraRotateAngle -= 10.0f;
	}
	else if (key == 'a')
	{
		keyState::a = keyState::a ? false : true;
	}
	else if (key == 's')
	{
		//keyState::t = false;
		keyState::t = false;
		keyState::m = false;
		keyState::a = false;
	}
	else if (key == 'c')
	{
		keyState::t = false;
		keyState::m = false;
		keyState::a = false;
		Brotate = false;
		Mrotate = false;
		Trotate = false;
		delta_x = 0.0f;
		mAngle = 0.0f;
		tAngle = 0.0f;
		camera_eye = { 200.0f, 100.0f, 200.0f };
		cameraAngle = 0.0f;
		cameraRotateAngle = 0.0f;
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


	glGenVertexArrays(1, &vao_lowerBody);
	glGenBuffers(2, vbo_lowerBody);

	glBindVertexArray(vao_lowerBody);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_lowerBody[1]);
	glBufferData(GL_ARRAY_BUFFER, lowerBody.color.size() * sizeof(GLfloat), lowerBody.color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_lowerBody[0]);
	glBufferData(GL_ARRAY_BUFFER, lowerBody.vertex.size() * sizeof(GLfloat), lowerBody.vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &vao_upperBody);
	glGenBuffers(2, vbo_upperBody);

	glBindVertexArray(vao_upperBody);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_upperBody[1]);
	glBufferData(GL_ARRAY_BUFFER, upperBody.color.size() * sizeof(GLfloat), upperBody.color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_upperBody[0]);
	glBufferData(GL_ARRAY_BUFFER, upperBody.vertex.size() * sizeof(GLfloat), upperBody.vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &vao_arm);
	glGenBuffers(2, vbo_arm);

	glBindVertexArray(vao_arm);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_arm[1]);
	glBufferData(GL_ARRAY_BUFFER, arm.color.size() * sizeof(GLfloat), arm.color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_arm[0]);
	glBufferData(GL_ARRAY_BUFFER, arm.vertex.size() * sizeof(GLfloat), arm.vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}