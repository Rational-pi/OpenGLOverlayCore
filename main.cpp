#include <windows.h>
#include "overlay.h"



int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str,int nWinMode) {
    //hide console window
    //HWND console = GetConsoleWindow();
    //ShowWindow(console,0);


    Overlay overlay(hThisInst," ",Overlay::Rect{0,0,100*1920/125,100*1080/125});
    overlay.init();
    overlay.Run();

    return (0);
}
