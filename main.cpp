#include <windows.h>
#include "overlay.h"
#include <ctime>



int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str,int nWinMode) {
    //hide console window
    //HWND console = GetConsoleWindow();
    //ShowWindow(console,0);


    Overlay overlay(hThisInst," ",Overlay::Rect{200,0,500,500});
    overlay.Run();

    return (0);
}
