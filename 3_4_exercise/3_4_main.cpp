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
GLboolean culling_on = true;
GLboolean fill_on = false;
GLboolean pause = false;
GLboolean key_c = false;
GLboolean key_t = false;
GLboolean key_m = false;
GLboolean key_r = false;

GLfloat left_scale = 1.0f;
GLfloat right_scale = 1.0f;
GLfloat all_scale = 1.0f;

GLuint vao[5];
GLuint vbo_axes[2];
GLuint vbo[2];
GLuint vbo_sphere[2];
GLuint vbo_cylinder[2];
GLuint vbo_cone[2];

unsigned int modelLocation;
unsigned int viewLocation;

glm::mat4 camera;

axes_coordination axes;
glm::mat4 tonado_transformation = glm::mat4(1.0f);
glm::mat4 hexa_trasformation = glm::mat4(1.0f);
glm::mat4 sphere_trasformation = glm::mat4(1.0f);

objRead sphere;
GLint sphere_size = sphere.loadObj_normalize_center("sphere.obj");

std::vector<GLfloat> tonado_vertex;

std::vector<GLfloat> hexa_vertex =
{
	//뒷면
	-0.1f,0.1f,0.1f,
	-0.1f,-0.1f,0.1f,
	0.1f,0.1f,0.1f,

	-0.1f,-0.1f,0.1f,
	0.1f,-0.1f,0.1f,
	0.1f,0.1f,0.1f,

	//왼쪽면
	0.1f,0.1f,0.1f,
	0.1f,-0.1f,0.1f,
	0.1f,0.1f,-0.1f,

	0.1f,-0.1f,0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f,0.1f,-0.1f,

	//앞면
	0.1f,0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	-0.1f,0.1f,-0.1f,

	0.1f,-0.1f,-0.1f,
	-0.1f,-0.1f,-0.1f,
	-0.1f,0.1f,-0.1f,

	//오른쪽면
	-0.1f,0.1f,-0.1f,
	-0.1f,-0.1f,-0.1f,
	-0.1f,0.1f,0.1f,

	-0.1f,-0.1f,-0.1f,
	-0.1f,-0.1f,0.1f,
	-0.1f,0.1f,0.1f,

	//윗면
	-0.1f,0.1f,-0.1f,
	-0.1f,0.1f,0.1f,
	0.1f,0.1f,-0.1f,

	-0.1f,0.1f,0.1f,
	0.1f,0.1f,0.1f,
	0.1f,0.1f,-0.1f,

	//아랫면
	-0.1f,-0.1f,0.1f,
	-0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,0.1f,

	-0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,-0.1f,
	0.1f,-0.1f,0.1f,
};
std::vector<GLfloat> hexa_color =
{
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,

	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,

	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,

	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,

	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,

	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
};

std::vector<GLfloat> obj_color =
{
	0.0f, 0.0f, 0.0f
};

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_4(15번)");

	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기

	GLint angle = 0;
	GLfloat radius = 0;

	for (int i = 0; i < 18 * 11 + 1; ++i) {
		tonado_vertex.push_back(radius * (cos(M_PI * angle / 180)));
		tonado_vertex.push_back(0.0f);
		tonado_vertex.push_back(radius * (sin(M_PI * angle / 180)));
		angle += 10;
		radius += 0.005;
	}
	
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기
	initBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, TimeEvent, 0);
	glutKeyboardFunc(KeyEvent);


	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	viewLocation = glGetUniformLocation(shaderID, "viewTransform");

	camera = glm::lookAt(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	//camera = glm::lookAt(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.0f, 0.2f, -0.0f));

	hexa_trasformation = glm::mat4(1.0f);
	hexa_trasformation = glm::translate(hexa_trasformation, glm::vec3(0.0f, 0.0f, -0.5f));


	sphere_trasformation = glm::mat4(1.0f);
	sphere_trasformation = glm::translate(sphere_trasformation, glm::vec3(0.0f, 0.0f, 0.5f));

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//랜더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderID);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));


	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	//좌표축 그리기

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	//회오리 그리기
	if (key_r)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tonado_transformation));
		glBindVertexArray(vao[3]);
		glDrawArrays(GL_LINE_STRIP, 0, tonado_vertex.size() / 3);
	}


		//육면체 그리기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(hexa_trasformation));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, hexa_vertex.size() / 3);

		//구 그리기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sphere_trasformation));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, sphere_size);
	

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLfloat left_obj_x = 0.0f;
GLfloat left_obj_y = 0.0f;
GLfloat left_obj_z = -0.5f;
GLfloat Lold_obj_x = left_obj_x;
GLfloat Lold_obj_y = left_obj_y;
GLfloat Lold_obj_z = left_obj_z;
GLfloat left_delta_x;
GLfloat left_delta_y;
GLfloat left_delta_z;
GLuint left_index = 0;


GLfloat right_obj_x = 0.0f;
GLfloat right_obj_y = 0.0f;
GLfloat right_obj_z = 0.5f;
GLfloat Rold_obj_x = right_obj_x;
GLfloat Rold_obj_y = right_obj_y;
GLfloat Rold_obj_z = right_obj_z;
GLfloat right_delta_x;
GLfloat right_delta_y;
GLfloat right_delta_z;
GLuint right_index = 3 * (9 * 11);

GLboolean t_state = true;
GLint t_time = 0;
GLboolean m_state = true;


GLvoid TimeEvent(int value)
{
	if (pause)
		return;

	hexa_trasformation = glm::mat4(1.0f);
	sphere_trasformation = glm::mat4(1.0f);


	if (key_t)
	{
		t_time++;
		if (t_state)
		{
			left_obj_x -= left_delta_x;
			left_obj_y -= left_delta_y;
			left_obj_z -= left_delta_z;
			right_obj_x -= right_delta_x;
			right_obj_y -= right_delta_y;
			right_obj_z -= right_delta_z;
		}
		else
		{
			left_obj_x += left_delta_x;
			left_obj_y += left_delta_y;
			left_obj_z += left_delta_z;
			right_obj_x += right_delta_x;
			right_obj_y += right_delta_y;
			right_obj_z += right_delta_z;
		}

		if (t_time % 100 == 0)
			t_state = t_state ? false : true;

	}
	else if (key_m)
	{
		t_time++;

		if (m_state) 
		{
			left_obj_x -= left_delta_x;
			left_obj_y -= left_delta_y;
			left_obj_z -= left_delta_z;
			right_obj_x -= right_delta_x;
			right_obj_y -= right_delta_y;
			right_obj_z -= right_delta_z;
		}
		else
		{
			left_obj_x += left_delta_x;
			left_obj_y += left_delta_y;
			left_obj_z += left_delta_z;
			right_obj_x += right_delta_x;
			right_obj_y += right_delta_y;
			right_obj_z += right_delta_z;
		}

		if (t_time % 100 == 0)
			m_state = m_state ? false : true;
	}
	else if (key_r)
	{
		left_index += 3;
		right_index += 3;

		if (left_index > 3 * (18 * 11))
			left_index = 0;
		
		if (right_index > 3 * (18 * 11))
			right_index = 0;

		left_obj_x = tonado_vertex[left_index];
		left_obj_z = tonado_vertex[left_index + 2];

		right_obj_x = tonado_vertex[right_index];
		right_obj_z = tonado_vertex[right_index + 2];
	}

	hexa_trasformation = glm::scale(hexa_trasformation, glm::vec3(all_scale, all_scale, all_scale));
	sphere_trasformation = glm::scale(sphere_trasformation, glm::vec3(all_scale, all_scale, all_scale));

	hexa_trasformation = glm::translate(hexa_trasformation, glm::vec3(left_obj_x, left_obj_y, left_obj_z));
	sphere_trasformation = glm::translate(sphere_trasformation, glm::vec3(right_obj_x, right_obj_y, right_obj_z));

	hexa_trasformation = glm::scale(hexa_trasformation, glm::vec3(left_scale, left_scale, left_scale));
	sphere_trasformation = glm::scale(sphere_trasformation, glm::vec3(right_scale, right_scale, right_scale));

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
	else if (key == 'p')
	{
		pause = pause ? false : true;
		if (!pause)
			glutTimerFunc(100, TimeEvent, 0);
	}
	else if (key == 'h')
	{
		culling_on = culling_on ? false : true;
		if (culling_on)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
	else if (key == 'x')
	{
		left_obj_x += 0.002f;
	}
	else if (key == 'X')
	{
		left_obj_x -= 0.002f;
	}
	else if (key == 'y')
	{
		left_obj_y += 0.002f;
	}
	else if (key == 'Y')
	{
		left_obj_y -= 0.002f;
	}
	else if (key == 'z')
	{
		left_obj_z += 0.002f;
	}
	else if (key == 'Z')
	{
		left_obj_z -= 0.002f;
	}
	else if (key == 'a')
	{
		right_obj_x += 0.002f;
	}
	else if (key == 'A')
	{
		right_obj_x -= 0.002f;
	}
	else if (key == 'b')
	{
		right_obj_y += 0.002f;
	}
	else if (key == 'B')
	{
		right_obj_y -= 0.002f;
	}
	else if (key == 'c')
	{
		right_obj_z += 0.002f;
	}
	else if (key == 'C')
	{
		right_obj_z -= 0.002f;
	}
	else if (key == 'g')
	{
		left_scale += 0.005f;
	}
	else if (key == 'G')
	{
		left_scale -= 0.005f;
	}
	else if (key == 'f')
	{
		right_scale += 0.005f;
	}
	else if (key == 'F')
	{
		right_scale -= 0.005f;
	}
	else if (key == '1')
	{
		all_scale += 0.005f;
	}
	else if (key == '2')
	{
		all_scale -= 0.005f;
	}
	else if (key == '3')
	{
		left_obj_y += 0.002f;
		right_obj_y += 0.002f;
		axes.transformations = glm::translate(axes.transformations, glm::vec3(0.0f, 0.002f, 0.0f));
		tonado_transformation = glm::translate(tonado_transformation, glm::vec3(0.0f, 0.002f, 0.0f));
	}
	else if (key == '4')
	{
		left_obj_y -= 0.002f;
		right_obj_y -= 0.002f;
		axes.transformations = glm::translate(axes.transformations, glm::vec3(0.0f, -0.002f, 0.0f));
		tonado_transformation = glm::translate(tonado_transformation, glm::vec3(0.0f, -0.002f, 0.0f));
	}
	else if (key == 'o')
	{
		key_c = key_c ? false : true;
	}
	else if (key == 'w')
	{
		fill_on = fill_on ? false : true;
		if (fill_on)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == 'r')
	{
		key_r = key_r ? false : true;
		if (key_r)
		{
			left_obj_x = tonado_vertex[0];
			left_obj_y = 0.0f;
			left_obj_z = tonado_vertex[2];

			right_obj_x = tonado_vertex[3 * (9 * 11)];
			right_obj_y = 0.0f;
			right_obj_z = tonado_vertex[3 * (9 * 11) + 2];
		}
		key_t = false;
		key_m = false;
		t_state = true;
		m_state = true;
	}
	else if (key == 's')
	{
		left_obj_x = 0.0f;
		left_obj_y = 0.0f;
		left_obj_z = -0.5f;
		right_obj_x = 0.0f;
		right_obj_y = 0.0f;
		right_obj_z = 0.5f;
		left_scale = 1.0f;
		right_scale = 1.0f;
		all_scale = 1.0f;
		axes.transformations = glm::mat4(1.0f);
		t_state = true;
		m_state = true;
		key_t = false;
		key_m = false;
		key_r = false;
		left_index = 0;
		right_index = 3 * (9 * 11);;
	}
	else if (key == 't')
	{
		key_t = key_t ? false : true;
		if (key_t)
		{
			t_time = 0;

			Lold_obj_x = left_obj_x;
			Lold_obj_y = left_obj_y;
			Lold_obj_z = left_obj_z;

			Rold_obj_x = right_obj_x;
			Rold_obj_y = right_obj_y;
			Rold_obj_z = right_obj_z;

			left_delta_x = left_obj_x / 100.0f;
			left_delta_y = left_obj_y / 100.0f;
			left_delta_z = left_obj_z / 100.0f;

			right_delta_x = right_obj_x / 100.0f;
			right_delta_y = right_obj_y / 100.0f;
			right_delta_z = right_obj_z / 100.0f;

		}
		key_m = false;
		m_state = true;
		key_r = false;
	}
	else if (key == 'm')
	{		
		key_m = key_m ? false : true;
		if (key_m)
		{
			t_time = 0;

			Lold_obj_x = left_obj_x;
			Lold_obj_y = left_obj_y;
			Lold_obj_z = left_obj_z;

			Rold_obj_x = right_obj_x;
			Rold_obj_y = right_obj_y;
			Rold_obj_z = right_obj_z;

			left_delta_x = (left_obj_x - right_obj_x) / 100.0f;
			left_delta_y = (left_obj_y - right_obj_y) / 100.0f;
			left_delta_z = (left_obj_z - right_obj_z) / 100.0f;

			right_delta_x = (right_obj_x - left_obj_x) / 100.0f;
			right_delta_y = (right_obj_y - left_obj_y) / 100.0f;
			right_delta_z = (right_obj_z - left_obj_z) / 100.0f;
		}
		key_t = false;
		t_state = true;
		key_r = false;
	}
}


void initBuffer()
{
	glGenVertexArrays(5, vao);
	glGenBuffers(2, vbo_axes);
	glGenBuffers(2, vbo);
	glGenBuffers(2, vbo_sphere);
	glGenBuffers(2, vbo_cylinder);
	glGenBuffers(2, vbo_cone);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, hexa_color.size() * sizeof(GLfloat), hexa_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, hexa_vertex.size() * sizeof(GLfloat), hexa_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[1]);
	glBufferData(GL_ARRAY_BUFFER, obj_color.size() * sizeof(GLfloat), obj_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[0]);
	glBufferData(GL_ARRAY_BUFFER, sphere.outvertex.size() * sizeof(glm::vec3), sphere.outvertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[3]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cylinder[1]);
	glBufferData(GL_ARRAY_BUFFER, obj_color.size() * sizeof(GLfloat), obj_color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cylinder[0]);
	glBufferData(GL_ARRAY_BUFFER, tonado_vertex.size() * sizeof(GLfloat), tonado_vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

}