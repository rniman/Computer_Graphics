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

GLuint vao[11];
GLuint vbo_axes[2];
GLuint plane_vbo[2*10];

unsigned int modelLocation;
unsigned int viewLocation;
unsigned int projLocation;

glm::mat4 camera;
glm::mat4 projection;
glm::mat4 view;

axes_coordination axes;
glm::mat4 hexa_trasformation = glm::mat4(1.0f);


class polygon
{
public:
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> color;
};

std::vector<std::vector<GLfloat>> hexa_plane =
{
	{
		//�ո�
		-100.0f,100.0f,100.0f,
		-100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f,

		-100.0f,-100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,100.0f
	},

	{
		//���ʸ�
		100.0f,100.0f,100.0f,
		100.0f,-100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		100.0f,-100.0f,100.0f,
		100.0f,-100.0f,-100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//�޸�
		100.0f,100.0f,-100.0f,
		100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f,

		100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,-100.0f
	},
	{
		//�����ʸ�
		-100.0f,100.0f,-100.0f,
		-100.0f,-100.0f,-100.0f,
		-100.0f,100.0f,100.0f,

		-100.0f,-100.0f,-100.0f,
		-100.0f,-100.0f,100.0f,
		-100.0f,100.0f,100.0f
	},
	{
		//����
		-100.0f,100.0f,-100.0f,
		-100.0f,100.0f,100.0f,
		100.0f,100.0f,-100.0f,

		-100.0f,100.0f,100.0f,
		100.0f,100.0f,100.0f,
		100.0f,100.0f,-100.0f
	},
	{
		//�Ʒ���
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


int main(int argc, char** argv)
{
	for (auto& e : hexa_plane[0])
	{
		std::cout << e << std::endl;
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example_3_5(16��)");

	glewExperimental = GL_TRUE;
	glewInit();

	//���̴� �о�ͼ� ���̴� ���α׷� �����

	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
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


	camera = glm::lookAt(glm::vec3(200.0f, 200.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.1f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.0f, 300.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.1f, 0.0f, -0.1f));
	//camera = glm::lookAt(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	//camera = glm::lookAt(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.0f, 0.2f, -0.0f));

	projection = glm::mat4(1.0f);
	//������� �����̰� ������� ����X
	projection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 50.0f, 600.0f);
	//projection = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 400.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));

	hexa_trasformation = glm::mat4(1.0f);
	hexa_trasformation = glm::translate(hexa_trasformation, glm::vec3(0.0f, 0.0f, -100.0f));

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
	
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	//��ǥ�� �׸���

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axes.transformations));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);


	//����ü �׸���
	for (int i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(hexa_trasformation));
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, hexa_plane[i].size() / 3);
	}
	

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid TimeEvent(int value)
{

	glutPostRedisplay();
	glutTimerFunc(100, TimeEvent, 0);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutExit();
}


void initBuffer()
{
	glGenVertexArrays(11, vao);
	glGenBuffers(2, vbo_axes);
	glGenBuffers(20, plane_vbo);

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

	glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, hexa_color[0].size() * sizeof(GLfloat), hexa_color[0].data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, plane_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, hexa_plane[0].size() * sizeof(GLfloat), hexa_plane[0].data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


}