#include <iostream>
#include <random>
#include <cmath>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid KeyEvent(unsigned char key, int x, int y);

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;
GLboolean timer_on = false;

std::random_device rd;
std::mt19937 gen(rd());

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ 
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 50); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example_1_1"); // 윈도우 생성
	
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyEvent);
	//glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value)
{
	if (timer_on)
	{
		std::uniform_int_distribution<int> dis(0, 100);
		red = static_cast<float>(dis(gen)) * 0.01f;
		red = floor(red * 100) / 100;
		green = static_cast<float>(dis(gen)) * 0.01f;
		green = floor(green * 100) / 100;
		blue = static_cast<float>(dis(gen)) * 0.01f;
		blue = floor(blue * 100) / 100;
		
		glutPostRedisplay();
		glutTimerFunc(500, TimerFunction, 1);
	}
	else
	{

	}
}

GLvoid KeyEvent(unsigned char key, int x, int y)
{
	if (key == 'r')			//빨간색
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	else if (key == 'g')	//초록색
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	else if (key == 'b')	//파랑색
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
	}
	else if (key == 'w')	//흰색
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
	}
	else if (key == 'k')	//검정색
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	else if (key == 'a')	//랜덤색
	{
		if (timer_on)
		{
			KeyEvent('s', 0, 0);
		}
		//소수점 3번째 버림
		std::uniform_int_distribution<int> dis(0, 100);
		red = static_cast<float>(dis(gen)) * 0.01f;
		red = floor(red * 100) / 100;
		green = static_cast<float>(dis(gen)) * 0.01f;
		green = floor(green * 100) / 100;
		blue = static_cast<float>(dis(gen)) * 0.01f;
		blue = floor(blue * 100) / 100;
	}
	else if (key == 't')	//타이머 호출 랜덤
	{
		if (!timer_on)
		{
			timer_on = timer_on ? false : true;
			glutTimerFunc(500, TimerFunction, 1);
		}
	}
	else if (key == 's')	//타이머 종료
	{
		if (timer_on)
		{
			timer_on = timer_on ? false : true;
			glutTimerFunc(500, TimerFunction, 1);
		}
	}
	else if (key == 'q')		//종료
	{
		glutLeaveMainLoop();
	}
	else
	{
		return;
	}

	//배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh
	glutPostRedisplay();
}