#ifndef OGLOW_H
#define OGLOW_H



#include <windows.h>
#include <chrono>// tick delta t
#include <string>// window name and id
#include <map>// per window id instance map
#include <iostream>

class OpenGL_OverlayWindow{
public:
    struct Rect{
        int x;
        int y;
        int w;
        int h;
    }rect;
private:
    static std::map<HWND,OpenGL_OverlayWindow*> instances;
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    HWND Window;
    HGLRC openglContextHandle;
    const char *windowClassName;
    HINSTANCE threadCaller;
    bool inited;
public:
    bool killme;
    OpenGL_OverlayWindow(HINSTANCE threadCaller,std::string WindowName,Rect rect);
    ~OpenGL_OverlayWindow();
    void init();
    void Tick();
    static LRESULT CALLBACK WindowClassMsgHandler(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam);
protected:
    virtual void GLInit()=0;
    virtual void GLTick(int64_t milli)=0;
    virtual void GLRender()=0;
    virtual void GLResize(int width,int height)=0;
};


#endif //OGLOW_H
