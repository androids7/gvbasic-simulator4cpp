#pragma once

#include <Windows.h>
#include <imm.h>

#pragma comment (lib, "imm32.lib")

class IME {
    HWND hwnd;
    HIMC himc;
public:
    IME(HWND);
    IME() : hwnd(0), himc(0) {}
    ~IME();
    void enable(); //�������뷨
    void disable(); //�������뷨
    void setHandle(HWND); //���ô��ھ��
    void setCandidatePos(int x, int y); //���ú�ѡ�ִ���λ��(client)
    void setCandidatePos(int x, int y, int rl, int rt, int rr, int rb);
};