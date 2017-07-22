#ifndef OGLOW_H
#define OGLOW_H

#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <dwmapi.h>
#include <tchar.h>
#include <assert.h>
#include <inttypes.h>
#include <chrono>



class OpenGL_OverlayWindow{
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    bool wellInit;
    HWND mainWindow;
    HGLRC openglContextHandle;
public:
    ~OpenGL_OverlayWindow(){
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(openglContextHandle) ;
    }

    OpenGL_OverlayWindow(HINSTANCE caller){

        int w = 240;
        int h = 240;
        const char szAppName[]="azeazsqdf";


        // WINDOW INNITIALISATION
        WNDCLASSEX app;
        memset(&app, 0, sizeof(app));
        app.cbSize = sizeof(WNDCLASSEX);
        app.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        app.style = CS_HREDRAW | CS_VREDRAW;
        app.lpfnWndProc = WindowFunction;
        app.cbClsExtra  = 0;
        app.cbWndExtra  = 0;
        app.hInstance = caller;
        app.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        app.hCursor = LoadCursor(NULL, IDC_ARROW);
        app.hbrBackground = (HBRUSH)CreateSolidBrush(0x00000000);
        app.lpszClassName = szAppName;//IDENTIFIER OF THE APP
        if(!RegisterClassEx(&app)) {MessageBox(NULL, _T("RegisterClassEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);wellInit=false;}
        mainWindow = CreateWindowEx(
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
                    caller,
                    NULL
                    );
        if(!mainWindow) {MessageBox(NULL, _T("CreateWindowEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);wellInit=false;}
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
        openglContextHandle = wglCreateContext(handelDC);
        wglMakeCurrent(handelDC, openglContextHandle);//make the stack thread opengl master
        GLInit();
        GLResize(w,h);
        ReleaseDC(mainWindow, handelDC);


    }




    void Tick(MSG &msg){
        std::chrono::time_point<std::chrono::system_clock> _now=std::chrono::high_resolution_clock::now();
        int64_t elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(_now-lastTime).count();
        lastTime=_now;

        GLTick(elapsed);


        // see if ther is a mesg for the window and dispatch it else
        if (PeekMessage(&msg, mainWindow, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
        //get overlay window device contect render and releas it
        SetWindowPos(mainWindow ,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);// make the window TOPMOST
        HDC hdc = GetDC(mainWindow);
        wglMakeCurrent(hdc, openglContextHandle);//make the stack thread opengl master current
        GLRender();
        SwapBuffers(hdc);
        ReleaseDC(mainWindow, hdc);
    }


    static LRESULT CALLBACK WindowFunction(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam) {
        switch(msg) {
        case WM_CREATE:
            break;
        case WM_DESTROY:
            PostQuitMessage(0) ;
            break;
        default:
            return DefWindowProc(hWnd,msg,wParam,lParam);
        }

        return 0;
    }

protected:
    virtual void GLInit();
    virtual void GLTick(int64_t milli);
    virtual void GLRender();
    virtual void GLResize(int width,int height);
};


#endif //OGLOW_H
