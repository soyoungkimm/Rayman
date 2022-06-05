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

#define Wsize 800
#define Hsize 200
#define PaletteSize 100000 // 팔레트 사이즈

float Wsize2 = Wsize / 2.;
float Hsize2 = Hsize / 2.;

int rx = 150, ry = 100; // 레이맨의 현재 좌표
int rayman_dir = 1;   // 0 : 왼쪽, 1 : 오른쪽
int fps = 0; //  frame 카운터
float fps_time = 1000.0 / 30.0; // 1 frame time
int rayman_frame = 0;     // 애니메이션 장면 위치
int rayman_jump_frame = 0;     // 애니메이션 점프 장면 위치
bool keys[256];
bool jump_on = false;
bool land_on = false;
int old_obj = -1;    // 없는 obj 번호로 초기화

// 팔레트 구조체
struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
Color pal[256];


// cel 구조체
struct cel_data
{
    short width; // 가로 길이
    short height; // 세로 길이
    unsigned char* imageData; // 이미지 데이터
};
cel_data rayman; // 레이맨
cel_data back; // 배경
cel_data stand[2]; // 레이맨 stand
cel_data walk[8]; // 레이맨 walk
cel_data craw[2]; // 레이맨 craw
cel_data jump[6]; // 레이맨 jump
cel_data land[3]; // 지형



// 오브젝트 구조체
struct object
{
    int kind;        // 물체종류
    int x, y, w, h;    // 출력좌표, 너비, 높이
};
object obj[20];
int n_obj;


// 충돌체 구조체
struct colider // 충돌 직사각형(충돌체)
{
    float left;
    float right;
    float top;
    float bottom;
};
colider raymanCol;
colider objCol[9];


// 걷기 애니메이션
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


// 기어다니기 애니메이션
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


// 충돌 처리(체크)
void collideCheck() {
    for (int i = 0; i < n_obj; i++)
    {
        if (raymanCol.left <= objCol[i].right && raymanCol.top >= objCol[i].bottom &&
            raymanCol.right >= objCol[i].left && raymanCol.bottom <= objCol[i].top)
        {
            //모든 물체들(obj[i])과 레이맨이 충돌하는지 조사하여 해당물체에 대한 처리
            land_on = true;
            if (i != old_obj) {
                // 충돌 처리
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


// 중력 처리
void gravity() {
    if (!land_on && !jump_on) {
        raymanCol.top += 5;
        raymanCol.bottom += 5;
        ry += 5;
    }
}


// 점프 중 올라갈 때
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


// 점프 중 내려갈 때
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


// 점프 중 컨트롤키랑 왼/오 눌렀는지 감지
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
    fps++; // frame 카운트
    fps = (fps + 1) % 65535;
    if (fps > 65535) fps = 0;     // 65535보다 커지면 0으로 초기화

    // --------------  키 입력 감지  ---------------
    // 점프 했을 때
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
    // 점프 안했을 때
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
        else if (keys[GLUT_KEY_LEFT]) // 왼쪽 키
        {
            rx -= 5;
            raymanCol.left -= 5;
            raymanCol.right -= 5;
            rayman_dir = 0;
            WalkAnimation(fps);
        }
        else if (keys[GLUT_KEY_RIGHT]) // 오른쪽 키
        {
            rx += 5;
            raymanCol.left += 5;
            raymanCol.right += 5;
            rayman_dir = 1;
            WalkAnimation(fps);
        }
        else // 그냥 서있을 때
        {
            if (fps % 5 == 0) {
                if (rayman_frame == 0) { // 수그리기
                    rayman_frame = 1;
                    rayman = stand[1];
                }
                else { // 서기
                    rayman_frame = 0;
                    rayman = stand[0];
                }
            }
        }
    }

    // 중력 처리
    gravity();

    // 충돌 판정
    collideCheck();

    glutPostRedisplay(); // dokeyboard 있는 것은 삭제
    glutTimerFunc(fps_time, Timer, 1); // 타이머 재호출
}


// 키 체크 관련 함수 -------------------------------------

// 일반 키 체크 함수
void Keyboard(unsigned char Key, int X, int Y)
{
    if (Key == 27) exit(0);    //ESC 누를시에 루프종료(게임 종료)
}


// 특수 키 체크 함수  // 키를 눌렀을 때
void DoKeyboard(int key, int x, int y)
{
    keys[key] = true;

    // ctrl 누름
    if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        keys[GLUT_ACTIVE_CTRL] = true;
    }

    // 현재 창 다시 표시하는 함수 : 이래야 레이맨이 움직임
    //glutPostRedisplay();
}

// 키를 놓았을 때
void doReleaseKey(int keyReleased, int x, int y)
{
    keys[keyReleased] = false;

    // ctrl 뗌
    if (keys[GLUT_ACTIVE_CTRL]) {
        keys[GLUT_ACTIVE_CTRL] = false;
    }
}


// 레이맨 충돌체 set 하는 함수
void set_rayman_colider()
{
    // Rayman 충돌 처리의 사각형(충돌체)은 stand[0]레이맨으로 한다.
    raymanCol.left = rx - (stand[0].width / 2);
    raymanCol.right = rx + (stand[0].width / 2);
    raymanCol.top = ry + 13 - 15;
    raymanCol.bottom = ry + 10 - 15;
}


// 지형 충돌체 set 하는 함수
void set_obj_colider()
{
    for (int i = 0; i < n_obj; i++) {
        objCol[i].left = obj[i].x - (obj[i].w / 2) + 40;
        objCol[i].right = obj[i].x + (obj[i].w / 2) + 40;
        objCol[i].top = (Hsize - obj[i].y) + 8 + 30;
        objCol[i].bottom = (Hsize - obj[i].y) + 30;
    }
}


// 맵 지형을 읽는 함수
void load_map() {

    // 파일 열기
    FILE* pFile;
    fopen_s(&pFile, "./data/map_n.txt", "r");

    // 오브젝트들 구조체에 set
    fscanf(pFile, "%d\n", &n_obj); // 물체 개수
    for (int i = 0; i < n_obj; i++) {
        fscanf(pFile, "%d %d %d %d %d\n", &obj[i].kind, &obj[i].x, &obj[i].y, &obj[i].w, &obj[i].h);
    }

    // 파일 닫기
    fclose(pFile);
}


// 배경 이미지를 읽는 함수
void load_back(char* filename)
{
    // 파일 기술자
    FILE* pFile;

    // 파일 열기
    char fileLocation[100] = "./data/";
    strcat(fileLocation, filename);
    fopen_s(&pFile, fileLocation, "rb"); // 파일 open

    // 이미지의 가로길이와 세로길이를 가져옴
    back.width = 320;
    back.height = 200;

    // 파일 읽어오기
    getc(pFile);
    getc(pFile);
    getc(pFile);
    getc(pFile);
    back.imageData = (unsigned char*)malloc(sizeof(unsigned char) * (back.width * back.height));
    fread(back.imageData, sizeof(unsigned char), back.width * back.height, pFile);

    // 파일 닫기
    fclose(pFile);
}


// 팔레트 읽는 함수
void load_pal(char* filename)
{
    // 파일 기술자
    FILE* pFile;

    // 팔레트 임시 배열
    unsigned char palette[PaletteSize];

    // 파일 열기
    char fileLocation[100] = "./data/";
    strcat(fileLocation, filename);
    fopen_s(&pFile, fileLocation, "rb"); // 파일 open

    // 파일 내용 읽기
    fread(palette, sizeof(unsigned char), PaletteSize, pFile);

    // 팔레트 set
    int plus = 0;
    for (int i = 0; i < 256; i++)
    {
        pal[i].red = palette[i + plus];
        pal[i].green = palette[i + plus + 1];
        pal[i].blue = palette[i + plus + 2];
        plus += 2;
    }
}


// 동적할당 해제 함수
void glutLeaveMainLoop()
{
    free(rayman.imageData);
    free(back.imageData);
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
    getc(pFile);

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

    if (color != 0) {
        glColor4f(pal[color].red / 255.0f, pal[color].green / 255.0f, pal[color].blue / 255.0f, 1.0f);// R,G,B (0 ~ 1)
        glVertex2f(nx, ny);
    }
}


// x, y 좌표에 cel그림 그리는 함수
void put_cel(int is_rayman, int dir, int x, int y, cel_data* cel)
{
    // 레이맨 일때
    if (is_rayman == 1) {
        x -= cel->width / 2;
        y -= cel->height;
    }

    //y = Hsize - y;

    int count = 0;
    for (int i = 0; i < cel->height; i++) {
        for (int j = 0; j < cel->width; j++) {
            if (dir == 0) set_pixel(x - j + cel->width, y + i, cel->imageData[count]); // 왼쪽 출력
            else set_pixel(x + j, y + i, cel->imageData[count]); // 오른쪽 출력
            count++;
        }
    }
}


// land 그리는 함수
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
// Render : 화면에 그래픽 출력
//------------------------------------------------------------------------------
void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);   // 활성화된 버퍼(칼러) 지우기

    glPointSize(1.0);   // 점크기지정

    glBegin(GL_POINTS); // 그래픽 시작

    // 배경 출력
    put_cel(0, 1, 0, 0, &back);

    // 맵 지형 출력
    put_land_cel();

    // 레이맨 출력
    put_cel(1, rayman_dir, rx, ry, &rayman);

    glEnd(); // 그래픽 끝

    glFlush();  // 지금까지의 내용을 실제 출력
}


//------------------------------------------------------------------------------
// Main : 메인
//------------------------------------------------------------------------------
int main()
{
    // atexit : 종료할 때 수행하는 함수
    atexit(glutLeaveMainLoop);

    // 레이맨 stand 이미지 읽어오기
    load_cel((char*)"STAND0.CEL", &stand[0]);
    load_cel((char*)"STAND1.CEL", &stand[1]);

    // 레이맨 walk 이미지 읽어오기
    load_cel((char*)"WALK0.CEL", &walk[0]);
    load_cel((char*)"WALK1.CEL", &walk[1]);
    load_cel((char*)"WALK2.CEL", &walk[2]);
    load_cel((char*)"WALK3.CEL", &walk[3]);
    load_cel((char*)"WALK4.CEL", &walk[4]);
    load_cel((char*)"WALK5.CEL", &walk[5]);
    load_cel((char*)"WALK6.CEL", &walk[6]);
    load_cel((char*)"WALK7.CEL", &walk[7]);

    // 레이맨 craw 이미지 읽어오기
    load_cel((char*)"CRAW0.CEL", &craw[0]);
    load_cel((char*)"CRAW1.CEL", &craw[1]);

    // 레이맨 jump 이미지 읽어오기
    load_cel((char*)"JUMP0.CEL", &jump[0]);
    load_cel((char*)"JUMP1.CEL", &jump[1]);
    load_cel((char*)"JUMP2.CEL", &jump[2]);
    load_cel((char*)"JUMP3.CEL", &jump[3]);
    load_cel((char*)"JUMP4.CEL", &jump[4]);
    load_cel((char*)"JUMP5.CEL", &jump[5]);

    // land 이미지 읽어오기
    load_cel((char*)"LAND0.CEL", &land[0]);
    load_cel((char*)"LAND1.CEL", &land[1]);
    load_cel((char*)"LAND2.CEL", &land[2]);

    // 팔레트 읽어오기
    load_pal((char*)"ALL.PAL");

    // 배경 읽어오기
    load_back((char*)"BACK30.CEL");

    // 맵 지형 읽어오기
    load_map();

    // 충돌체 set
    set_rayman_colider();
    set_obj_colider();


    glutInitDisplayMode(GLUT_RGBA); // RGB 그래픽모드 지정

    glutInitWindowPosition(0, 0);            // 창위치
    glutInitWindowSize(Wsize, Hsize);       // 창크기
    glutCreateWindow("Gamejigi DOS Rayman 교육");     // 창제목

    glClearColor(0.0, 0.0, 1.0, 0.0);  // R,G,B,A 배경:파란색

    glutTimerFunc(fps_time, Timer, 1); //타이머: fps초 뒤에 Timer 함수 실행

    // 키 체크
    glutKeyboardFunc(Keyboard); // 일반 키 체크
    glutSpecialFunc(DoKeyboard); // 특수키 체크
    glutSpecialUpFunc(doReleaseKey); // 키보드를 놓았을때 작동

    glutDisplayFunc(Render); // 화면출력

    glutMainLoop(); // 대기상태
}