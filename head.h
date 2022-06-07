#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "glext.h"
#include <string.h> 

#define Wsize 320   
#define Hsize 200
#define PaletteSize 100000 
#define JumpPower 35

// 팔레트 구조체
struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};


// cel 구조체
struct cel_data
{
    short width; // 가로 길이
    short height; // 세로 길이
    unsigned char* imageData; // 이미지 데이터
};


// 오브젝트 구조체
struct object
{
    int kind;        // 물체종류
    int x, y, w, h;    // 출력좌표, 너비, 높이
};


// 충돌체 구조체
struct colider // 충돌 직사각형(충돌체)
{
    float left;
    float right;
    float top;
    float bottom;
};


void WalkAnimation(int fps);
void CrawAnimation(int fps);
void StandAnimation(int fps);
void set_rayman_colider();
void set_obj_colider();
void collideCheck();
void gravity();
void jump_up();
void jump_down();
void check_ctrl_left_right();
void Timer(int Value);
void Keyboard(unsigned char Key, int X, int Y);
void DoKeyboard(int key, int x, int y);
void doReleaseKey(int keyReleased, int x, int y);
void load_map();
void load_back(char* filename);
void load_pal(char* filename);
void load_cel(char* filename, cel_data* cel);
void glutLeaveMainLoop();
void set_pixel(int x, int y, unsigned char color);
void put_cel(int is_rayman, int dir, int x, int y, cel_data* cel);
void put_land_cel();
void Render();