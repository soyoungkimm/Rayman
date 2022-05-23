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
#include <string.h> 

#define Wsize 320
#define Hsize 200
#define PaletteSize 100000 // �ȷ�Ʈ ������

float Wsize2 = Wsize / 2.;
float Hsize2 = Hsize / 2.;

int rx = 20, ry = 20; // ���̸��� ���� ��ǥ

// �ȷ�Ʈ ����ü
struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
Color pal[256];


// cel ����ü
struct cel_data
{
    short width; // ���� ����
    short height; // ���� ����
    unsigned char* imageData; // �̹��� ������
};
cel_data rayman; // ���̸� stand
cel_data back; // ���


// ��� �̹����� �д� �Լ�
void load_back(char* filename)
{
    // ���� �����
    FILE* pFile;

    // ���� ����
    char fileLocation[100] = "./data/";
    strcat(fileLocation, filename);
    fopen_s(&pFile, fileLocation, "rb"); // ���� open

    // �̹����� ���α��̿� ���α��̸� ������
    back.width = 320;
    back.height = 200;

    // ���� �о����
    back.imageData = (unsigned char*)malloc(sizeof(unsigned char) * (back.width * back.height));
    fread(back.imageData, sizeof(unsigned char), back.width * back.height, pFile);

    // ���� �ݱ�
    fclose(pFile);
}


// �ȷ�Ʈ �д� �Լ�
void load_pal(char* filename)
{
    // ���� �����
    FILE* pFile;

    // �ȷ�Ʈ �ӽ� �迭
    unsigned char palette[PaletteSize];

    // ���� ����
    char fileLocation[100] = "./data/";
    strcat(fileLocation, filename);
    fopen_s(&pFile, fileLocation, "rb"); // ���� open

    // ���� ���� �б�
    fread(palette, sizeof(unsigned char), PaletteSize, pFile);

    // �ȷ�Ʈ set
    int plus = 0;
    for (int i = 0; i < 256; i++)
    {
        pal[i].red = palette[i + plus];
        pal[i].green = palette[i + plus + 1];
        pal[i].blue = palette[i + plus + 2];
        plus += 2;
    }
}


// �����Ҵ� ���� �Լ�
void glutLeaveMainLoop()
{
    free(rayman.imageData);
}


// �׸� �ڷḦ �д� �Լ�
void load_cel(char* filename, cel_data* cel)
{
    // ���� �����
    FILE* pFile;

    // ���� ����
    char fileLocation[100] = "./data/";
    strcat(fileLocation, filename);
    fopen_s(&pFile, fileLocation, "rb"); // ���� open

    // �̹����� ���α��̿� ���α��̸� ������
    cel->width = getc(pFile);
    getc(pFile);
    cel->height = getc(pFile);

    // ���� �о����
    cel->imageData = (unsigned char*)malloc(sizeof(unsigned char) * (cel->width * cel->height));
    fread(cel->imageData, sizeof(unsigned char), cel->width * cel->height, pFile);

    // ���� �ݱ�
    fclose(pFile);
}


//------------------------------------------------------------------------------
// set_pixel : �� ��� �Լ�
//------------------------------------------------------------------------------
void set_pixel(int x, int y, unsigned char color)
{
    GLfloat nx = (x - Wsize2) / Wsize2;   // x��ǥ(0~Wsize)�� gl��ǥ(-1~ +1)�� ��ȯ
    GLfloat ny = -(y - Hsize2) / Hsize2;  // y��ǥ(0~Hsize)�� gl��ǥ(-1~ +1)�� ��ȯ

    if (color != 0) {
        glColor4f(pal[color].red / 255.0f, pal[color].green / 255.0f, pal[color].blue / 255.0f, 1.0f);// R,G,B (0 ~ 1)
        glVertex2f(nx, ny);
    }
}


// x, y ��ǥ�� cel�׸� �׸��� �Լ�
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
// Render : ȭ�鿡 �׷��� ���
//------------------------------------------------------------------------------
void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);   // Ȱ��ȭ�� ����(Į��) �����

    glPointSize(1.0);   // ��ũ������

    glBegin(GL_POINTS); // �׷��� ����

    // ��� ���
    put_cel(0, 0, &back);

    // ���̸� ���
    put_cel(rx, ry, &rayman);

    glEnd();            // �׷��� ��

    glFlush();  // ���ݱ����� ������ ���� ���
}


//------------------------------------------------------------------------------
// Main : ����
//------------------------------------------------------------------------------
int main()
{
    // atexit : ������ �� �����ϴ� �Լ� 
    atexit(glutLeaveMainLoop);

    // ���̸� �̹��� �о����
    load_cel((char*)"STAND0.CEL", &rayman);

    // �ȷ�Ʈ �о����
    load_pal((char*)"ALL.PAL");

    // ��� �о����
    load_back((char*)"BACK30.CEL");

    glutInitDisplayMode(GLUT_RGBA); // RGB �׷��ȸ�� ����

    glutInitWindowPosition(0, 0);            // â��ġ
    glutInitWindowSize(Wsize, Hsize);       // âũ��
    glutCreateWindow("Gamejigi DOS Rayman ����");     // â����

    glClearColor(0.0, 0.0, 0.0, 0.0);  // R,G,B,A ���:�Ķ��� 

    glutDisplayFunc(Render); // ȭ�����

    glutMainLoop(); // ������
}