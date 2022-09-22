#include <iostream>
#include <random>
#include <cmath>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

const GLint window_w = 800;
const GLint window_h = 600;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid MouseEvent(int button, int state, int x, int y);
void convert_WindowXY_OpenglXY(const int& x,const int& y, float* ox, float* oy);

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;

std::random_device rd;
std::mt19937 gen(rd());

class Center_Rect
{
public:
	GLint left, right;
	GLint top, bottom;
	GLfloat red = 0.0f;
	GLfloat green = 0.0f;
	GLfloat blue = 0.0f;
	
	GLfloat glLeft, glRight;
	GLfloat glTop, glBottom;

	//gl좌표 set
	void setGLPos();
};

void Center_Rect::setGLPos()
{
	convert_WindowXY_OpenglXY(left, bottom, &glLeft, &glBottom);
	convert_WindowXY_OpenglXY(right, top, &glRight, &glTop);
}

Center_Rect cenRect = { 300,500,200,400 };

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 50); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example_1_2"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	cenRect.setGLPos();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(MouseEvent);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	glColor3f(cenRect.red, cenRect.green, cenRect.blue);
	glRectf(cenRect.glLeft, cenRect.glBottom, cenRect.glRight, cenRect.glTop);

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid MouseEvent(int button, int state, int x, int y)
{
	//x,y는 윈도우 좌표값을 받는다 
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//왼쪽클릭
		//1.사각형 내부 클릭
		if(x < cenRect.right && x > cenRect.left && y > cenRect.top && y < cenRect.bottom)
		{ 
			std::uniform_int_distribution<int> dis(0, 100);
			cenRect.red = static_cast<float>(dis(gen)) * 0.01f;
			cenRect.red = floor(cenRect.red * 100) / 100;
			cenRect.green = static_cast<float>(dis(gen)) * 0.01f;
			cenRect.green = floor(cenRect.green * 100) / 100;
			cenRect.blue = static_cast<float>(dis(gen)) * 0.01f;
			cenRect.blue = floor(cenRect.blue * 100) / 100;
		}
		//2.사각형 외부 클릭
		else
		{
			std::uniform_int_distribution<int> dis(0, 100);
			red = static_cast<float>(dis(gen)) * 0.01f;
			red = floor(red * 100) / 100;
			green = static_cast<float>(dis(gen)) * 0.01f;
			green = floor(green * 100) / 100;
			blue = static_cast<float>(dis(gen)) * 0.01f;
			blue = floor(blue * 100) / 100;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//오른쪽클릭
		//1.사각형 내부 클릭
		if (x < cenRect.right && x > cenRect.left && y > cenRect.top && y < cenRect.bottom)
		{
			if (cenRect.top <= 10 || cenRect.left <= 10)
				return;

			cenRect.left -= 10;
			cenRect.right += 10;
			cenRect.top -= 10;
			cenRect.bottom += 10;

			cenRect.setGLPos();
		}
		//2.사각형 외부 클릭
		else
		{
			if (cenRect.right - cenRect.left <= 20)
				return;

			cenRect.left += 10;
			cenRect.right -= 10;
			cenRect.top += 10;
			cenRect.bottom -= 10;

			cenRect.setGLPos();
		}
	}

	glutPostRedisplay();
}

//윈도우 좌표계에 만들어진 점을 GL좌표계로 바꾸는 함수
void convert_WindowXY_OpenglXY(const int& x,const int& y, float* ox, float* oy)
{
	*ox = (float)((x - (float)window_w / 2.0) * (float)(1.0 / (float)(window_w / 2.0)));
	*oy = -(float)((y - (float)window_h / 2.0) * (float)(1.0 / (float)(window_h / 2.0)));
}