#pragma once

#include <Windows.h>
#include "gvb.h"

struct Dev : Device {
private:
    static const int bmask[];
    int tmode;

    void setPoint(coord x, coord y);
    void hLine(coord x1, coord x2, coord y);
    void ovalPoint(coord ox, coord oy, coord x, coord y);
    void moveLine();
public:
    Dev();

    void appendText(const string &);
    void nextLine();
    void updateLCD();
    void locate(int row, int col);
    int getX();
    int getY();
    void setMode(int mode);
    void cls();
    string input(const string &prompt, int type);
    int getkey();
    void point(coord x, coord y, int mode);
    void rectangle(coord x1, coord y1, coord x2, coord y2, bool fill, int mode);
    void line(coord x1, coord y1, coord x2, coord y2, int mode);
    void ellipse(coord x, coord y, coord rx, coord ry, bool fill, int mode);

    int peek(int addr);
    string peek(int, int size);
    void poke(int addr, byte value);
    void poke(int addr, const char *, int size);
    void call(int addr);

    void sleep(int ms);
    void paint(int addr, int x, int y, byte w, byte h, int mode);
    bool getPoint(int x, int y);
    bool checkKey(int keycode);
};

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK scrnProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK kbdProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK aboutDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK modifyDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK modArryDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK optionDlgProc(HWND, UINT, WPARAM, LPARAM);

void insertCols();
void refreshItems();

DWORD WINAPI run(LPVOID);
void waitThread();
void CALLBACK blink(HWND, UINT, UINT_PTR, DWORD); //�����˸

#define SCRNBGCOLOR 0xffffff
#define SCRNFRCOLOR 0

enum POS_INFO {
    IS_ASCII, IS_GB_0, IS_GB_1
};
POS_INFO getPosInfo(); //��ȡ���λ����Ϣ
void refresh();
void refresh(int left, int top, int right, int bottom);
void clearCursor();

bool loadFont(); //�����ֿ�
void initOFN(HWND);
void initDIB(); 
void uninitDIB();
bool loadFile(const char *); //�������

int getKeyValue(int x, int y); //��ȡ���������Ӧ�ļ�ֵ
int getKeyValue(int pckey); //��ȡpc��ֵ��Ӧ��wqx��ֵ
int mappingKey(int wqxkey); //��ȡ��ֵӳ��λ��x yy

//wqx������Ϣ�����͵�����  lParam:wqx��ֵ
#define CM_KEYPRESS WM_USER
#define CM_KEYDOWN (WM_USER + 1)
#define CM_KEYUP (WM_USER + 2)

void saveDIB(std::ostream &);
void dumpSyntaxTree(std::ostream &, GVB &);
void saveDebugInfo(std::ostream &, GVB &);