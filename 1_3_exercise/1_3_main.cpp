#include <iostream>
#include <random>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

const GLint window_w = 800;
const GLint window_h = 600;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);
GLvoid MouseClick(int button, int state, int x, int y);
GLvoid MouseMove(int x, int y);

GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy);

struct rect
{
	GLfloat x1 = 0 - 0.3f, x2 = 0 + 0.3f;
	GLfloat y1 = 0 - 0.4f, y2 = 0 + 0.4f;

	GLfloat red = 0.0f, green = 1.0f, blue = 1.0f;
	GLboolean empty = false;
	GLboolean click = false;
};

rect rect_array[5];

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;
GLint num_rect = 1; //사각형 개수
GLboolean left_click = false;
GLint oldX, oldY;

std::random_device rd;
std::mt19937 gen(rd());

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 50); // 윈도우의 위치 지정
	glutInitWindowSize(window_w, window_h); // 윈도우의 크기 지정
	glutCreateWindow("Example_1_3"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	rect_array[0].empty = true;

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyEvent);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	for (auto& e : rect_array)
	{
		if (!e.empty)
			break;

		glColor3f(e.red, e.green, e.blue);
		glRectf(e.x1, e.y1, e.x2, e.y2);
	}


	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'a' && num_rect < 5)
	{
		std::uniform_int_distribution<int> dis(0, 100);
		rect_array[num_rect].empty = true;
		rect_array[num_rect].red = (float)dis(gen) / 100;
		rect_array[num_rect].green = (float)dis(gen) / 100;
		rect_array[num_rect].blue = (float)dis(gen) / 100;
		num_rect++;

		glutPostRedisplay();
	}
}

GLvoid MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		oldX = x;
		oldY = y;

		for (int i = 4; i >= 0; --i)
		{
			GLint tx1, tx2;
			GLint ty1, ty2;
			if (!rect_array[i].empty)
				continue;
		
			convert_OpenglXY_WindowXY(tx1, ty1, rect_array[i].x1, rect_array[i].y1);
			convert_OpenglXY_WindowXY(tx2, ty2, rect_array[i].x2, rect_array[i].y2);

			if (x > tx1 && x < tx2 && y < ty1 && y > ty2)
			{
				left_click = true;
				rect_array[i].click = true;
				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		left_click = false;
		for (auto& e : rect_array)
			e.click = false;
	}
}

GLvoid MouseMove(int x, int y)
{
	if (left_click)
	{
		for (auto& e : rect_array)
		{
			if (!e.empty)
				break;

			if (!e.click)
				continue;
			
			e.x1 -= (float)(oldX - x) / (float)(window_w / 2);
			e.x2 -= (float)(oldX - x) / (float)(window_w / 2);
			//y축은 부호가 반대
			e.y1 += (float)(oldY - y) / (float)(window_h / 2);
			e.y2 += (float)(oldY - y) / (float)(window_h / 2);

			oldX = x;
			oldY = y;
		}

		glutPostRedisplay();
	}
}

//변환 생각하기 어렵구만
GLvoid convert_OpenglXY_WindowXY(int& x, int& y, const float& ox, const float& oy)
{
	//*ox = (float)((x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0)));
	//*oy = -(float)((y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0)));
	x = static_cast<int>((window_w / 2) + ox * (window_w / 2));
	y = static_cast<int>((window_h / 2) - oy * (window_h / 2));
}
