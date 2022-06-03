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

int rx = 80, ry = 80; // ���̸��� ���� ��ǥ
int rayman_dir = 1;   // 0 : ����, 1 : ������
int fps = 0; //  frame ī����
float fps_time = 1000.0 / 30.0; // 1 frame time
int rayman_frame = 0;     // �ִϸ��̼� ��� ��ġ
bool keys[256];
bool jump_on = false;

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
cel_data rayman; // ���̸�
cel_data back; // ���
cel_data stand[2]; // ���̸� stand
cel_data walk[8]; // ���̸� walk
cel_data craw[2]; // ���̸� craw
cel_data jump[6]; // ���̸� jump


void WalkAnimation(int fps) {
    if (rayman_frame >= 8) rayman_frame = 0;

    if (fps % 3 == 0) {
        if (rayman_frame == 0) {
            rayman_frame = 1;
            rayman = walk[0];
        }
        else if (rayman_frame == 1) {
            rayman_frame = 2;
            rayman = walk[1];
        }
        else if (rayman_frame == 2) {
            rayman_frame = 3;
            rayman = walk[2];
        }
        else if (rayman_frame == 3) {
            rayman_frame = 4;
            rayman = walk[3];
        }
        else if (rayman_frame == 4) {
            rayman_frame = 5;
            rayman = walk[4];
        }
        else if (rayman_frame == 5) {
            rayman_frame = 6;
            rayman = walk[5];
        }
        else if (rayman_frame == 6) {
            rayman_frame = 7;
            rayman = walk[6];
        }
        else if (rayman_frame == 7) {
            rayman_frame = 0;
            rayman = walk[7];
        }
    }
}


void CrawAnimation(int fps) {
    if (fps % 3 == 0) {
        if (rayman_frame == 0) {
            rayman_frame = 1;
            rayman = craw[0];
        }
        else {
            rayman_frame = 0;
            rayman = craw[1];
        }
    }
}


void Timer(int Value)
{
    fps++; // frame ī��Ʈ
    fps = (fps + 1) % 65535;
    if (fps > 65535) fps = 0;     // 65535���� Ŀ���� 0���� �ʱ�ȭ

    // --------------  Ű �Է� ����  ---------------
    // ���� ���� ��
    if (jump_on)
    {
        if (fps % 4 == 0) {

            if (rayman_frame == 8) {
                ry -= 20;
                if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) rx -= 10;
                else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) rx += 10;
                rayman_frame = 9;
                rayman = jump[1];
            }
            else if (rayman_frame == 9) {
                ry -= 20;
                if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) rx -= 10;
                else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) rx += 10;
                rayman_frame = 10;
                rayman = jump[2];
            }
            else if (rayman_frame == 10) {
                ry += 20;
                if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) rx -= 10;
                else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) rx += 10;
                rayman_frame = 11;
                rayman = jump[3];
            }
            else if (rayman_frame == 11) {
                ry += 20;
                if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) rx -= 10;
                else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) rx += 10;
                rayman_frame = 12;
                rayman = jump[4];
            }
            else if (rayman_frame == 12) {
                ry += 20;
                if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) rx -= 10;
                else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) rx += 10;
                rayman_frame = 13;
                rayman = jump[5];
                jump_on = false;
            }
            else {
                ry -= 20;
                if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) rx -= 10;
                else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) rx += 10;
                rayman_frame = 8;
                rayman = jump[0];
            }
        }
    }
    // ���� ������ ��
    else
    {
        if (keys[GLUT_KEY_DOWN] && keys[GLUT_KEY_LEFT]) {
            rx -= 5;
            rayman_dir = 0;

            CrawAnimation(fps);
        }
        else if (keys[GLUT_KEY_DOWN] && keys[GLUT_KEY_RIGHT]) {
            rx += 5;
            rayman_dir = 1;

            CrawAnimation(fps);
        }
        else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) {
            rayman_dir = 0;
            jump_on = true;
        }
        else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) {
            rayman_dir = 1;
            jump_on = true;
        }
        else if (keys[GLUT_KEY_UP]) {
            jump_on = true;
        }
        else if (keys[GLUT_KEY_LEFT]) // ���� Ű
        {
            rx -= 5;
            rayman_dir = 0;

            WalkAnimation(fps);
        }
        else if (keys[GLUT_KEY_RIGHT]) // ������ Ű
        {
            rx += 5;
            rayman_dir = 1;

            WalkAnimation(fps);
        }
        else // �׳� ������ ��
        {
            if (fps % 5 == 0) {
                if (rayman_frame == 0) { // ���׸���
                    rayman_frame = 1;
                    rayman = stand[1];
                }
                else { // ����
                    rayman_frame = 0;
                    rayman = stand[0];
                }
            }
        }
    }

    glutPostRedisplay(); // dokeyboard �ִ� ���� ����
    glutTimerFunc(fps_time, Timer, 1); // Ÿ�̸� ��ȣ��
}


// Ű üũ ���� �Լ� -------------------------------------

// �Ϲ� Ű üũ �Լ�
void Keyboard(unsigned char Key, int X, int Y)
{
    if (Key == 27) exit(0);    //ESC �����ÿ� ��������(���� ����)
}


// Ư�� Ű üũ �Լ�  // Ű�� ������ ��
void DoKeyboard(int key, int x, int y)
{
    keys[key] = true;

    // ctrl ����
    if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        keys[GLUT_ACTIVE_CTRL] = true;
    }

    // ���� â �ٽ� ǥ���ϴ� �Լ� : �̷��� ���̸��� ������
    //glutPostRedisplay();
}

// Ű�� ������ ��
void doReleaseKey(int keyReleased, int x, int y)
{
    keys[keyReleased] = false;

    // ctrl ��
    if (keys[GLUT_ACTIVE_CTRL]) {
        keys[GLUT_ACTIVE_CTRL] = false;
    }
}


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
    free(back.imageData);
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
    getc(pFile);

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
void put_cel(int is_rayman, int dir, int x, int y, cel_data* cel)
{
    // ���̸� �϶�
    if (is_rayman == 1) {
        x -= cel->width / 2;
        y -= cel->height;
    }

    int count = 0;
    for (int i = 0; i < cel->height; i++) {
        for (int j = 0; j < cel->width; j++) {
            if (dir == 0) set_pixel(x - j + cel->width, y + i, cel->imageData[count]); // ���� ���
            else set_pixel(x + j, y + i, cel->imageData[count]); // ������ ���
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
    put_cel(0, 1, 0, 0, &back);

    // ���̸� ���
    put_cel(1, rayman_dir, rx, ry, &rayman);

    glEnd(); // �׷��� ��

    glFlush();  // ���ݱ����� ������ ���� ���
}


//------------------------------------------------------------------------------
// Main : ����
//------------------------------------------------------------------------------
int main()
{
    // atexit : ������ �� �����ϴ� �Լ�
    atexit(glutLeaveMainLoop);

    // ���̸� stand �̹��� �о����
    load_cel((char*)"STAND0.CEL", &stand[0]);
    load_cel((char*)"STAND1.CEL", &stand[1]);

    // ���̸� walk �̹��� �о����
    load_cel((char*)"WALK0.CEL", &walk[0]);
    load_cel((char*)"WALK1.CEL", &walk[1]);
    load_cel((char*)"WALK2.CEL", &walk[2]);
    load_cel((char*)"WALK3.CEL", &walk[3]);
    load_cel((char*)"WALK4.CEL", &walk[4]);
    load_cel((char*)"WALK5.CEL", &walk[5]);
    load_cel((char*)"WALK6.CEL", &walk[6]);
    load_cel((char*)"WALK7.CEL", &walk[7]);

    // ���̸� craw �̹��� �о����
    load_cel((char*)"CRAW0.CEL", &craw[0]);
    load_cel((char*)"CRAW1.CEL", &craw[1]);

    // ���̸� jump �̹��� �о����
    load_cel((char*)"JUMP0.CEL", &jump[0]);
    load_cel((char*)"JUMP1.CEL", &jump[1]);
    load_cel((char*)"JUMP2.CEL", &jump[2]);
    load_cel((char*)"JUMP3.CEL", &jump[3]);
    load_cel((char*)"JUMP4.CEL", &jump[4]);
    load_cel((char*)"JUMP5.CEL", &jump[5]);

    // �ȷ�Ʈ �о����
    load_pal((char*)"ALL.PAL");

    // ��� �о����
    load_back((char*)"BACK30.CEL");

    glutInitDisplayMode(GLUT_RGBA); // RGB �׷��ȸ�� ����

    glutInitWindowPosition(0, 0);            // â��ġ
    glutInitWindowSize(Wsize, Hsize);       // âũ��
    glutCreateWindow("Gamejigi DOS Rayman ����");     // â����

    glClearColor(0.0, 0.0, 0.0, 0.0);  // R,G,B,A ���:�Ķ���

    glutTimerFunc(fps_time, Timer, 1); //Ÿ�̸�: fps�� �ڿ� Timer �Լ� ����

    // Ű üũ
    glutKeyboardFunc(Keyboard); // �Ϲ� Ű üũ
    glutSpecialFunc(DoKeyboard); // Ư��Ű üũ
    glutSpecialUpFunc(doReleaseKey); // Ű���带 �������� �۵�

    glutDisplayFunc(Render); // ȭ�����

    glutMainLoop(); // ������
}