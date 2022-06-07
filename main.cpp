//==============================================================================
// ����: Gamejigi DOS Rayman ����(OpenGl ����)
// 
// �Ҽ�: �δ����б� ��ǻ�ͼ���Ʈ����� ������ 
// ��¥: 1998.10-2018.5, 2021.07.07
// �̸�: ���� ������, ������ 17�� ������(OpenGl ��ȯ)
//==============================================================================
#include "head.h"

extern float fps_time;
extern cel_data stand[2];
extern cel_data walk[8];
extern cel_data craw[2];
extern cel_data jump[6];
extern cel_data land[3];

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

    glutInitWindowPosition(0, 0); // â��ġ
    glutInitWindowSize(Wsize, Hsize); // âũ��
    glutCreateWindow("Gamejigi DOS Rayman ����"); // â����

    glClearColor(0.0, 0.0, 1.0, 0.0);  // R,G,B,A ���:�Ķ��� 
    glutTimerFunc(fps_time, Timer, 1); //Ÿ�̸�: fps�� �ڿ� Timer �Լ� ����

    // Ű üũ 
    glutKeyboardFunc(Keyboard); // �Ϲ� Ű üũ
    glutSpecialFunc(DoKeyboard); // Ư��Ű üũ
    glutSpecialUpFunc(doReleaseKey); // Ű���带 �������� �۵�

    glutDisplayFunc(Render); // ȭ�����

    glutMainLoop(); // ������
}