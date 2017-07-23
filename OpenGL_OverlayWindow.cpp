#include "OpenGL_OverlayWindow.h"

#include <dwmapi.h>

std::map<HWND,OpenGL_OverlayWindow*> OpenGL_OverlayWindow::instances=std::map<HWND,OpenGL_OverlayWindow*>();


OpenGL_OverlayWindow::OpenGL_OverlayWindow(HINSTANCE caller, std::string WindowName, Rect rect){
    killme=false;
    this->threadCaller=caller;
    windowClassName=WindowName.c_str();


    // WINDOW INNITIALISATION
    WNDCLASSEX app;
    memset(&app, 0, sizeof(app));
    app.cbSize = sizeof(WNDCLASSEX);
    app.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    app.style = CS_HREDRAW | CS_VREDRAW;
    app.lpfnWndProc = OpenGL_OverlayWindow::WindowClassMsgHandler;
    app.cbClsExtra  = 0;
    app.cbWndExtra  = 0;
    app.hInstance = threadCaller;
    app.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    app.hCursor = LoadCursor(NULL, IDC_ARROW);
    app.hbrBackground = (HBRUSH)CreateSolidBrush(0x00000000);
    app.lpszClassName = windowClassName;//IDENTIFIER OF THE APP
    if(!RegisterClassEx(&app)) {MessageBox(NULL, "RegisterClassEx - failed", "Error", MB_OK | MB_ICONERROR);killme=true;}
    Window = CreateWindowEx(
                WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
                windowClassName,
                ("Overlay Window - "+WindowName).c_str(),
                WS_VISIBLE | WS_POPUP,
                rect.x,
                rect.y,
                rect.w,
                rect.h,
                NULL,
                NULL,
                threadCaller,
                NULL
                );
    if(!Window) {MessageBox(NULL, "CreateWindowEx - failed", "Error", MB_OK | MB_ICONERROR);killme=true;}
    DWM_BLURBEHIND bb = {0};
    bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    bb.hRgnBlur = CreateRectRgn(0, 0, -1, -1);
    bb.fEnable = TRUE;
    DwmEnableBlurBehindWindow(Window, &bb);



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

    HDC handelDC = GetDC(Window);
    SetPixelFormat(handelDC, ChoosePixelFormat(handelDC, &pfd), &pfd);
    openglContextHandle = wglCreateContext(handelDC);
    wglMakeCurrent(handelDC, openglContextHandle);//make the stack thread opengl master
    GLInit();
    GLResize(rect.w,rect.h);
    ReleaseDC(Window, handelDC);


    instances[Window]=this;
}

OpenGL_OverlayWindow::~OpenGL_OverlayWindow(){
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(openglContextHandle) ;
    instances.erase(Window);
    DestroyWindow(Window);
    UnregisterClass(windowClassName,threadCaller);
}

void OpenGL_OverlayWindow::Tick(){
    /// GLTick(timeelapsed in milliseconds)
    std::chrono::time_point<std::chrono::system_clock> _now=std::chrono::high_resolution_clock::now();
    GLTick(std::chrono::duration_cast<std::chrono::milliseconds>(_now-lastTime).count());
    lastTime=_now;


    /// GLRender()
    //get overlay window device contect render and releas it
    SetWindowPos(Window ,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);// make the window TOPMOST
    HDC hdc = GetDC(Window);
    wglMakeCurrent(hdc, openglContextHandle);//make the stack thread opengl master current
    GLRender();
    SwapBuffers(hdc);
    ReleaseDC(Window, hdc);


    // Handle window messages
    MSG msg; if (PeekMessage(&msg, Window, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
}

LRESULT OpenGL_OverlayWindow::WindowClassMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            break;
        case WM_DESTROY:
            for (std::pair<HWND,OpenGL_OverlayWindow*> instance : instances) {
                if (instance.first==hWnd)instance.second->killme=true;
            }break;
        default:
            return DefWindowProc(hWnd,msg,wParam,lParam);
    }

    return 0;
}
