//==============================================================================
// 제목: Gamejigi DOS Rayman 교육(OpenGl 버전)
// 
// 소속: 인덕대학교 컴퓨터소프트웨어과 겜지기 
// 날짜: 1998.10-2018.5, 2021.07.07
// 이름: 교수 윤형태, 겜지기 17기 최지웅(OpenGl 변환)
//==============================================================================
#include "head.h"

extern float fps_time;
extern cel_data stand[2];
extern cel_data walk[8];
extern cel_data craw[2];
extern cel_data jump[6];
extern cel_data land[3];

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

    glutInitWindowPosition(0, 0); // 창위치
    glutInitWindowSize(Wsize, Hsize); // 창크기
    glutCreateWindow("Gamejigi DOS Rayman 교육"); // 창제목

    glClearColor(0.0, 0.0, 1.0, 0.0);  // R,G,B,A 배경:파란색 
    glutTimerFunc(fps_time, Timer, 1); //타이머: fps초 뒤에 Timer 함수 실행

    // 키 체크 
    glutKeyboardFunc(Keyboard); // 일반 키 체크
    glutSpecialFunc(DoKeyboard); // 특수키 체크
    glutSpecialUpFunc(doReleaseKey); // 키보드를 놓았을때 작동

    glutDisplayFunc(Render); // 화면출력

    glutMainLoop(); // 대기상태
}