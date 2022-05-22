//==============================================================================
// 제목: Gamejigi DOS Rayman 교육(OpenGl 버전)
// 
// 소속: 인덕대학교 컴퓨터소프트웨어과 겜지기 
// 날짜: 1998.10-2018.5, 2021.07.07
// 이름: 교수 윤형태, 겜지기 17기 최지웅(OpenGl 변환)
//==============================================================================
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "glext.h"

#define Wsize 320
#define Hsize 200

float Wsize2 = Wsize / 2.;
float Hsize2 = Hsize / 2.;

//------------------------------------------------------------------------------
// set_pixel : 점 찍는 함수
//------------------------------------------------------------------------------
void set_pixel(int x, int y, unsigned char color)
{
    GLfloat nx = (x - Wsize2) / Wsize2;   // x좌표(0~Wsize)를 gl좌표(-1~ +1)로 변환
    GLfloat ny = -(y - Hsize2) / Hsize2;  // y좌표(0~Hsize)를 gl좌표(-1~ +1)로 변환

    glColor3f(color / 255.0f, color / 255.0f, color / 255.0f);     // R,G,B (0 ~ 1)
    glVertex2f(nx, ny);
}


//------------------------------------------------------------------------------
// Render : 화면에 그래픽 출력
//------------------------------------------------------------------------------
void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);   // 활성화된 버퍼(칼러) 지우기

    glPointSize(1.0);   // 점크기지정

    glBegin(GL_POINTS); // 그래픽 시작

    set_pixel(50, 50, 255);     // 50,50 좌표에 255밝기로 점 출력

    glEnd();            // 그래픽 끝

    glFlush();  // 지금까지의 내용을 실제 출력
}

//------------------------------------------------------------------------------
// Main : 메인
//------------------------------------------------------------------------------
int main()
{
    glutInitDisplayMode(GLUT_RGBA); // RGB 그래픽모드 지정

    glutInitWindowPosition(0, 0);            // 창위치
    glutInitWindowSize(Wsize, Hsize);       // 창크기
    glutCreateWindow("Gamejigi DOS Rayman 교육");     // 창제목

    glClearColor(0.0, 0.0, 1.0, 0.0);  // R,G,B,A 배경:파란색 

    glutDisplayFunc(Render); // 화면출력

    glutMainLoop(); // 대기상태
}