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

// �ȷ�Ʈ ����ü
struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};


// cel ����ü
struct cel_data
{
    short width; // ���� ����
    short height; // ���� ����
    unsigned char* imageData; // �̹��� ������
};


// ������Ʈ ����ü
struct object
{
    int kind;        // ��ü����
    int x, y, w, h;    // �����ǥ, �ʺ�, ����
};


// �浹ü ����ü
struct colider // �浹 ���簢��(�浹ü)
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