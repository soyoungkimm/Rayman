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
#define JumpPower 35

float Wsize2 = Wsize / 2.;
float Hsize2 = Hsize / 2.;

int rx = 50, ry = 150; // ���̸��� ���� ��ǥ
int rayman_dir = 1;   // 0 : ����, 1 : ������
int fps = 0; //  frame ī����
float fps_time = 1000.0 / 30.0; // 1 frame time
int rayman_frame = 0;     // �ִϸ��̼� ��� ��ġ
int rayman_jump_frame = 0;     // �ִϸ��̼� ���� ��� ��ġ
bool keys[256];
bool jump_on = false;
bool land_on = false;
int old_obj = -1;    // ���� obj ��ȣ�� �ʱ�ȭ
int scroll_x = 0, scroll_y = 0; // ��ũ�� ����



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
cel_data land[3]; // ����


// ������Ʈ ����ü
struct object
{
    int kind;        // ��ü����
    int x, y, w, h;    // �����ǥ, �ʺ�, ����
};
object obj[20];
int n_obj;


// �浹ü ����ü
struct colider // �浹 ���簢��(�浹ü)
{
    float left;
    float right;
    float top;
    float bottom;
};
colider raymanCol;
colider objCol[9];


// �ȱ� �ִϸ��̼�
void WalkAnimation(int fps) {
    if (rayman_frame >= 8) rayman_frame = 0;

    if (fps % 3 == 0) {
        rayman = walk[rayman_frame];
        rayman_frame++;
    }
}


// ���ٴϱ� �ִϸ��̼�
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


void StandAnimation(int fps) {
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


// ���̸� �浹ü set �ϴ� �Լ�
void set_rayman_colider()
{
    // Rayman �浹 ó���� �簢��(�浹ü)�� stand[0]���̸����� �Ѵ�.
    raymanCol.left = (rx - scroll_x) - (stand[0].width / 2);
    raymanCol.right = (rx - scroll_x) + (stand[0].width / 2);
    raymanCol.top = (ry + scroll_y) + 13 - 15;
    raymanCol.bottom = (ry + scroll_y) + 10 - 15;
}


// ���� �浹ü set �ϴ� �Լ�
void set_obj_colider()
{
    for (int i = 0; i < n_obj; i++) {
        objCol[i].left = obj[i].x - scroll_x;
        objCol[i].right = (obj[i].x - scroll_x) + obj[i].w;
        objCol[i].top = (Hsize - (obj[i].y - scroll_y)) + 8 + 30;
        objCol[i].bottom = (Hsize - (obj[i].y - scroll_y)) + 30;
    }
}


// �浹 ó��(üũ)
void collideCheck() {
    for (int i = 0; i < n_obj; i++)
    {
        //��� ��ü��(obj[i])�� ���̸��� �浹�ϴ��� �����Ͽ� �ش繰ü�� ���� ó��
        if (raymanCol.left <= objCol[i].right && raymanCol.top >= objCol[i].bottom &&
            raymanCol.right >= objCol[i].left && raymanCol.bottom <= objCol[i].top)
        {
            land_on = true;
            if (i != old_obj) {
                // �浹 ó��
                old_obj = i;
                jump_on = false;
            }
            // ���� �ε��� ��ü�� ���ִ� ��?�̸�
            if (i == 8 || i == 7 || i == 6 || i == 5) {
                rayman_jump_frame = 0;
            }
            break;
        }
        else {
            land_on = false;
        }
    }
}


// �߷� ó��
void gravity() {
    if (!land_on && !jump_on) {
        ry += 5;
    }
}


// ���� �� �ö� ��
void jump_up() {
    for (int i = 0; i < JumpPower; i++) {
        collideCheck();
        if (jump_on) {
            ry -= 1;
            set_rayman_colider();
        }
        else {
            break;
        }
    }
}


// ���� �� ������ ��
void jump_down() {
    for (int i = 0; i < JumpPower; i++) {
        collideCheck();
        if (jump_on) {
            ry += 1;
            set_rayman_colider();
        }
        else {
            break;
        }
    }
}


// ���� �� ��Ʈ��Ű�� ��/�� �������� ����
void check_ctrl_left_right() {
    if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_LEFT]) {
        rx -= 10;
    }
    else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) {
        rx += 10;
    }
}


void Timer(int Value)
{
    fps++; // frame ī��Ʈ
    fps = (fps + 1) % 65535;
    if (fps > 65535) fps = 0;     // 65535���� Ŀ���� 0���� �ʱ�ȭ

    // --------------  Ű �Է� ����  ---------------

    // ���߿��� �������ϰ� ����
    if (rayman_jump_frame == 0 && !land_on) {
        StandAnimation(fps);
    }
    // ���� ���� ��
    else if (jump_on)
    {
        if (fps % 4 == 0) {
            if (rayman_jump_frame == 0) {
                jump_up();
                check_ctrl_left_right();
                rayman_jump_frame = 1;
                rayman = jump[0];
            }
            else if (rayman_jump_frame == 1) {
                jump_up();
                check_ctrl_left_right();
                rayman_jump_frame = 2;
                rayman = jump[1];
            }
            else if (rayman_jump_frame == 2) {
                jump_up();
                check_ctrl_left_right();
                rayman_jump_frame = 3;
                rayman = jump[2];
            }
            else if (rayman_jump_frame == 3) {
                jump_down();
                check_ctrl_left_right();
                rayman_jump_frame = 4;
                rayman = jump[3];
            }
            else if (rayman_jump_frame == 4) {
                jump_down();
                check_ctrl_left_right();
                rayman_jump_frame = 5;
                rayman = jump[4];
            }
            else if (rayman_jump_frame == 5) {
                jump_down();
                check_ctrl_left_right();
                rayman_jump_frame = 0;
                rayman = jump[5];
                jump_on = false;
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
            StandAnimation(fps);
        }
    }

    // ---------------------- ȭ�� ��ũ�� ------------------------

    // �������� ������ ����
    if (rx < 40) {
        rx = 40;
    }

    // ������ ������ ������ - ���������� ��ũ��
    if (rx - scroll_x > 280) {
        scroll_x += 5;
    }

    // �������� ������ ������ - �������� ��ũ��
    // rx > 50 : ó���� ������ �� �������� ���°� ����
    if (rx > 50 && rx - scroll_x < 50) {
        scroll_x -= 5;
    }

    // ���� ���� - ���� ��ũ��
    if (Hsize / 2 > (ry + scroll_y) && (ry + scroll_y) < Hsize) {
        scroll_y += 5;
    }

    // ������ �������� - ������ ��ũ��
    if (150 > (ry + scroll_y) && (ry + scroll_y) >= 100 && ry < 100) {
        scroll_y -= 5;
    }

    // -------------------------------------------------------

    // colider �� ����
    set_rayman_colider();
    set_obj_colider();

    // �浹 ����
    collideCheck();

    // �߷� ó��
    gravity();

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


// �� ������ �д� �Լ�
void load_map() {

    // ���� ����
    FILE* pFile;
    fopen_s(&pFile, "./data/map_n.txt", "r");

    // ������Ʈ�� ����ü�� set
    fscanf(pFile, "%d\n", &n_obj); // ��ü ����
    for (int i = 0; i < n_obj; i++) {
        fscanf(pFile, "%d %d %d %d %d\n", &obj[i].kind, &obj[i].x, &obj[i].y, &obj[i].w, &obj[i].h);
    }

    // ���� �ݱ�
    fclose(pFile);
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
    fseek(pFile, 4, SEEK_SET);
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


// land �׸��� �Լ�
void put_land_cel() {
    int count = 0;
    for (int k = 0; k < n_obj; k++) {
        for (int i = 0; i < land[obj[k].kind].height; i++) {
            for (int j = 0; j < land[obj[k].kind].width; j++) {
                set_pixel((obj[k].x - scroll_x) + j, Hsize - (obj[k].y - scroll_y) + i, land[obj[k].kind].imageData[count]);
                count++;
            }
        }
        count = 0;
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

    // �� ���� ���
    put_land_cel();

    // ���̸� ���
    put_cel(1, rayman_dir, rx - scroll_x, ry + scroll_y, &rayman);

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

    // land �̹��� �о����
    load_cel((char*)"LAND0.CEL", &land[0]);
    load_cel((char*)"LAND1.CEL", &land[1]);
    load_cel((char*)"LAND2.CEL", &land[2]);

    // �ȷ�Ʈ �о����
    load_pal((char*)"ALL.PAL");

    // ��� �о����
    load_back((char*)"BACK30.CEL");

    // �� ���� �о����
    load_map();

    // �浹ü set
    set_rayman_colider();
    set_obj_colider();


    glutInitDisplayMode(GLUT_RGBA); // RGB �׷��ȸ�� ����

    glutInitWindowPosition(0, 0);            // â��ġ
    glutInitWindowSize(Wsize, Hsize);       // âũ��
    glutCreateWindow("Gamejigi DOS Rayman ����");     // â����

    glClearColor(0.0, 0.0, 1.0, 0.0);  // R,G,B,A ���:�Ķ���
    glutTimerFunc(fps_time, Timer, 1); //Ÿ�̸�: fps�� �ڿ� Timer �Լ� ����

    // Ű üũ
    glutKeyboardFunc(Keyboard); // �Ϲ� Ű üũ
    glutSpecialFunc(DoKeyboard); // Ư��Ű üũ
    glutSpecialUpFunc(doReleaseKey); // Ű���带 �������� �۵�

    glutDisplayFunc(Render); // ȭ�����

    glutMainLoop(); // ������
}