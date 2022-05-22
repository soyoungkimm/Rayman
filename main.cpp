//==============================================================================
// ����: Gamejigi DOS Rayman ����(OpenGl ����)
// 
// �Ҽ�: �δ����б� ��ǻ�ͼ���Ʈ����� ������ 
// ��¥: 1998.10-2018.5, 2021.07.07
// �̸�: ���� ������, ������ 17�� ������(OpenGl ��ȯ)
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
// set_pixel : �� ��� �Լ�
//------------------------------------------------------------------------------
void set_pixel(int x, int y, unsigned char color)
{
    GLfloat nx = (x - Wsize2) / Wsize2;   // x��ǥ(0~Wsize)�� gl��ǥ(-1~ +1)�� ��ȯ
    GLfloat ny = -(y - Hsize2) / Hsize2;  // y��ǥ(0~Hsize)�� gl��ǥ(-1~ +1)�� ��ȯ

    glColor3f(color / 255.0f, color / 255.0f, color / 255.0f);     // R,G,B (0 ~ 1)
    glVertex2f(nx, ny);
}


//------------------------------------------------------------------------------
// Render : ȭ�鿡 �׷��� ���
//------------------------------------------------------------------------------
void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);   // Ȱ��ȭ�� ����(Į��) �����

    glPointSize(1.0);   // ��ũ������

    glBegin(GL_POINTS); // �׷��� ����

    set_pixel(50, 50, 255);     // 50,50 ��ǥ�� 255���� �� ���

    glEnd();            // �׷��� ��

    glFlush();  // ���ݱ����� ������ ���� ���
}

//------------------------------------------------------------------------------
// Main : ����
//------------------------------------------------------------------------------
int main()
{
    glutInitDisplayMode(GLUT_RGBA); // RGB �׷��ȸ�� ����

    glutInitWindowPosition(0, 0);            // â��ġ
    glutInitWindowSize(Wsize, Hsize);       // âũ��
    glutCreateWindow("Gamejigi DOS Rayman ����");     // â����

    glClearColor(0.0, 0.0, 1.0, 0.0);  // R,G,B,A ���:�Ķ��� 

    glutDisplayFunc(Render); // ȭ�����

    glutMainLoop(); // ������
}