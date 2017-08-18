#include <windows.h>
#include "overlay.h"



int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str,int nWinMode) {
    //hide console window
    //HWND console = GetConsoleWindow();
    //ShowWindow(console,0);

    Overlay overlay(hThisInst," ",Overlay::Rect{0,0,50,50});
    return (overlay.Run());
}
