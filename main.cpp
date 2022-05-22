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
#include <string.h>

#define Wsize 320
#define Hsize 200

float Wsize2 = Wsize / 2.;
float Hsize2 = Hsize / 2.;

int rx = 10, ry = 10; // 레이맨의 현재 좌표

// cel 구조체
struct cel_data
{
    short width; // 가로 길이
    short height; // 세로 길이
    unsigned char* imageData; // 이미지 데이터
};
cel_data rayman;


// 동적할당 해제 함수
void glutLeaveMainLoop()
{
    free(rayman.imageData);
}


// 그림 자료를 읽는 함수
void load_cel(char* filename, cel_data* cel)
{
    // 파일 기술자
    FILE* pFile;

    // 파일 열기
    char fileLocation[100] = "./data/";
    strcat(fileLocation, filename);
    fopen_s(&pFile, fileLocation, "rb"); // 파일 open

    // 이미지의 가로길이와 세로길이를 가져옴
    cel->width = getc(pFile);
    getc(pFile);
    cel->height = getc(pFile);

    // 파일 읽어오기
    cel->imageData = (unsigned char*)malloc(sizeof(unsigned char) * (cel->width * cel->height));
    fread(cel->imageData, sizeof(unsigned char), cel->width * cel->height, pFile);

    // 파일 닫기
    fclose(pFile);
}



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


// x, y 좌표에 cel그림 그리는 함수
void put_cel(int x, int y, cel_data* cel)
{
    int count = 0;
    for (int i = 0; i < cel->height; i++) {
        for (int j = 0; j < cel->width; j++) {
            set_pixel(x + j, y + i, cel->imageData[count]);
            count++;
        }
    }
}


//------------------------------------------------------------------------------
// Render : 화면에 그래픽 출력
//------------------------------------------------------------------------------
void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);   // 활성화된 버퍼(칼러) 지우기

    glPointSize(1.0);   // 점크기지정

    glBegin(GL_POINTS); // 그래픽 시작

    // 레이맨 출력
    put_cel(rx, ry, &rayman);

    glEnd();            // 그래픽 끝

    glFlush();  // 지금까지의 내용을 실제 출력
}


//------------------------------------------------------------------------------
// Main : 메인
//------------------------------------------------------------------------------
int main()
{
    // atexit : 종료할 때 수행하는 함수 
    atexit(glutLeaveMainLoop);

    // 레이맨 이미지 읽어오기
    char fileName[12] = "STAND0.CEL";
    load_cel(fileName, &rayman);

    glutInitDisplayMode(GLUT_RGBA); // RGB 그래픽모드 지정

    glutInitWindowPosition(0, 0);            // 창위치
    glutInitWindowSize(Wsize, Hsize);       // 창크기
    glutCreateWindow("Gamejigi DOS Rayman 교육");     // 창제목

    glClearColor(0.0, 0.0, 0.0, 0.0);  // R,G,B,A 배경:파란색

    glutDisplayFunc(Render); // 화면출력

    glutMainLoop(); // 대기상태
}