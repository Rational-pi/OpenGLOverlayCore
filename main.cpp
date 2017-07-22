//#define _WIN32_WINNT 0x0500
#include <iostream>

#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <dwmapi.h>
#include <tchar.h>
#include <assert.h>

#include "opengl.h"

Opengl gl;
bool quit=false;



uint64_t c=0;
LRESULT CALLBACK WindowFunction(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam) {
    std::cout << c++ << " " << msg <<std::endl;
    switch(msg) {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        quit=true;
        PostQuitMessage(0) ;
        break;
    default:
        return DefWindowProc(hWnd,msg,wParam,lParam);
    }

    return 0;
}



int WINAPI _tWinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str,int nWinMode) {
    //hide console window
    HWND console = GetConsoleWindow();
    ShowWindow(console,0);



    /*//hoock the slave window id
    HWND slaveWin=FindWindow(NULL,L"Untitled - Notepad");
    //get its rect
    if(slaveWin!=NULL){
        LPRECT rc;
        GetWindowRect(slaveWin,&rc);
        w =  rc.right - rc.left;
        h = rc.bottom - rc.top;
        SetWindowPos(slaveWin ,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    }else{
        ExitProcess(0);
    }*/

    int w = 240;
    int h = 240;


    // WINDOW INNITIALISATION
    const char szAppName[]="azeazsqdf";
    WNDCLASSEX app;
    memset(&app, 0, sizeof(app));
    app.cbSize = sizeof(WNDCLASSEX);
    app.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    app.style = CS_HREDRAW | CS_VREDRAW;
    app.lpfnWndProc = WindowFunction;
    app.cbClsExtra  = 0;
    app.cbWndExtra  = 0;
    app.hInstance = hThisInst;
    app.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    app.hCursor = LoadCursor(NULL, IDC_ARROW);
    app.hbrBackground = (HBRUSH)CreateSolidBrush(0x00000000);
    app.lpszClassName = szAppName;//IDENTIFIER OF THE APP
    if(!RegisterClassEx(&app)) {MessageBox(NULL, _T("RegisterClassEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);return false;}
    HWND mainWindow = CreateWindowEx(
                WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
                szAppName,
                _T("Overlay Window"),
                WS_VISIBLE | WS_POPUP,
                0,
                0,
                w,
                h,
                NULL,
                NULL,
                hThisInst,
                NULL
                );
    if(!mainWindow) {MessageBox(NULL, _T("CreateWindowEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);return FALSE;}
    DWM_BLURBEHIND bb = {0};
    bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    bb.hRgnBlur = CreateRectRgn(0, 0, -1, -1);
    bb.fEnable = TRUE;
    DwmEnableBlurBehindWindow(mainWindow, &bb);












    // init opengl context and ling it to "mainWindow"
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,                                // Version Number
        PFD_DRAW_TO_WINDOW      |         // Format Must Support Window
        PFD_SUPPORT_OPENGL      |         // Format Must Support OpenGL
        PFD_SUPPORT_COMPOSITION |         // Format Must Support Composition
        PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
        PFD_TYPE_RGBA,                    // Request An RGBA Format
        32,                               // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
        8,                                // An Alpha Buffer
        0,                                // Shift Bit Ignored
        0,                                // No Accumulation Buffer
        0, 0, 0, 0,                       // Accumulation Bits Ignored
        24,                               // 16Bit Z-Buffer (Depth Buffer)
        8,                                // Some Stencil Buffer
        0,                                // No Auxiliary Buffer
        PFD_MAIN_PLANE,                   // Main Drawing Layer
        0,                                // Reserved
        0, 0, 0                           // Layer Masks Ignored
    };

    HDC handelDC = GetDC(mainWindow);
    SetPixelFormat(handelDC, ChoosePixelFormat(handelDC, &pfd), &pfd);
    HGLRC openglContextHandle = wglCreateContext(handelDC);
    wglMakeCurrent(handelDC, openglContextHandle);//make the stack thread opengl master
    gl.Init();
    gl.Resize(w,h);
    ReleaseDC(mainWindow, handelDC);



    // LOOP
    MSG msg;
    while(!quit) {
        // see if ther is a mesg for the window and dispatch it else
        if (PeekMessage(&msg, mainWindow, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
        //get overlay window device contect render and releas it
        SetWindowPos(mainWindow ,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);// make the window TOPMOST
        HDC hdc = GetDC(mainWindow);
        wglMakeCurrent(hdc, openglContextHandle);//make the stack thread opengl master current
        gl.Render();
        SwapBuffers(hdc);
        ReleaseDC(mainWindow, hdc);
    }

    // releas opengl context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(openglContextHandle) ;

    // quit
    return (FALSE);
}
