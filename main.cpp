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

#define Wsize 800
#define Hsize 200
#define PaletteSize 100000 // �ȷ�Ʈ ������

float Wsize2 = Wsize / 2.;
float Hsize2 = Hsize / 2.;

int rx = 150, ry = 100; // ���̸��� ���� ��ǥ
int rayman_dir = 1;   // 0 : ����, 1 : ������
int fps = 0; //  frame ī����
float fps_time = 1000.0 / 30.0; // 1 frame time
int rayman_frame = 0;     // �ִϸ��̼� ��� ��ġ
int rayman_jump_frame = 0;     // �ִϸ��̼� ���� ��� ��ġ
bool keys[256];
bool jump_on = false;
bool land_on = false;
int old_obj = -1;    // ���� obj ��ȣ�� �ʱ�ȭ

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


// �浹 ó��(üũ)
void collideCheck() {
    for (int i = 0; i < n_obj; i++)
    {
        if (raymanCol.left <= objCol[i].right && raymanCol.top >= objCol[i].bottom &&
            raymanCol.right >= objCol[i].left && raymanCol.bottom <= objCol[i].top)
        {
            //��� ��ü��(obj[i])�� ���̸��� �浹�ϴ��� �����Ͽ� �ش繰ü�� ���� ó��
            land_on = true;
            if (i != old_obj) {
                // �浹 ó��
                old_obj = i;
                jump_on = false;
            }
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
        raymanCol.top += 5;
        raymanCol.bottom += 5;
        ry += 5;
    }
}


// ���� �� �ö� ��
void jump_up() {
    for (int i = 0; i < 40; i++) {
        collideCheck();
        if (jump_on) {
            ry -= 1;
            raymanCol.top -= 1;
            raymanCol.bottom -= 1;
        }
        else {
            break;
        }
    }
}


// ���� �� ������ ��
void jump_down() {
    for (int i = 0; i < 40; i++) {
        collideCheck();
        if (jump_on) {
            ry += 1;
            raymanCol.top += 1;
            raymanCol.bottom += 1;
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
        raymanCol.left -= 10;
        raymanCol.right -= 10;
    }
    else if (keys[GLUT_ACTIVE_CTRL] && keys[GLUT_KEY_RIGHT]) {
        rx += 10;
        raymanCol.left += 10;
        raymanCol.right += 10;
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
            if (rayman_jump_frame == 8) {
                jump_up();
                check_ctrl_left_right();
                rayman_jump_frame = 9;
                rayman = jump[1];
            }
            else if (rayman_jump_frame == 9) {
                jump_up();
                check_ctrl_left_right();
                rayman_jump_frame = 10;
                rayman = jump[2];
            }
            else if (rayman_jump_frame == 10) {
                jump_down();
                check_ctrl_left_right();
                rayman_jump_frame = 11;
                rayman = jump[3];
            }
            else if (rayman_jump_frame == 11) {
                jump_down();
                check_ctrl_left_right();
                rayman_jump_frame = 12;
                rayman = jump[4];
            }
            else if (rayman_jump_frame == 12) {
                jump_down();
                check_ctrl_left_right();
                rayman_jump_frame = 13;
                rayman = jump[5];
                jump_on = false;
            }
            else {
                jump_up();
                check_ctrl_left_right();
                rayman_jump_frame = 8;
                rayman = jump[0];
            }
        }
    }
    // ���� ������ ��
    else
    {
        if (keys[GLUT_KEY_DOWN] && keys[GLUT_KEY_LEFT]) {
            rx -= 5;
            raymanCol.left -= 5;
            raymanCol.right -= 5;
            rayman_dir = 0;
            CrawAnimation(fps);
        }
        else if (keys[GLUT_KEY_DOWN] && keys[GLUT_KEY_RIGHT]) {
            rx += 5;
            raymanCol.left += 5;
            raymanCol.right += 5;
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
            raymanCol.left -= 5;
            raymanCol.right -= 5;
            rayman_dir = 0;
            WalkAnimation(fps);
        }
        else if (keys[GLUT_KEY_RIGHT]) // ������ Ű
        {
            rx += 5;
            raymanCol.left += 5;
            raymanCol.right += 5;
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

    // �߷� ó��
    gravity();

    // �浹 ����
    collideCheck();

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


// ���̸� �浹ü set �ϴ� �Լ�
void set_rayman_colider()
{
    // Rayman �浹 ó���� �簢��(�浹ü)�� stand[0]���̸����� �Ѵ�.
    raymanCol.left = rx - (stand[0].width / 2);
    raymanCol.right = rx + (stand[0].width / 2);
    raymanCol.top = ry + 13 - 15;
    raymanCol.bottom = ry + 10 - 15;
}


// ���� �浹ü set �ϴ� �Լ�
void set_obj_colider()
{
    for (int i = 0; i < n_obj; i++) {
        objCol[i].left = obj[i].x - (obj[i].w / 2) + 40;
        objCol[i].right = obj[i].x + (obj[i].w / 2) + 40;
        objCol[i].top = (Hsize - obj[i].y) + 8 + 30;
        objCol[i].bottom = (Hsize - obj[i].y) + 30;
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
    getc(pFile);
    getc(pFile);
    getc(pFile);
    getc(pFile);
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

    //y = Hsize - y;

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
                set_pixel(obj[k].x + j, Hsize - obj[k].y + i, land[obj[k].kind].imageData[count]);
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