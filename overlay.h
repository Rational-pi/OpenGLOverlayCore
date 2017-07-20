#ifndef OVERLAY_H
#define OVERLAY_H

#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <dwmapi.h>
#include <tchar.h>
#include <assert.h>
#include <inttypes.h>
#include <chrono>

#define clock std::chrono::time_point<std::chrono::system_clock>
#define now(a) std::chrono::high_resolution_clock::now(a)
#define toms(dur) std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()




class Overlay
{
public:
    struct WinMainArgs {
        HINSTANCE hThisInst;
        HINSTANCE hPrevInst;
        LPSTR str;
        int nWinMode;
    };
    struct OpenglFunctions {
        void (*Init)();
        void (*Resize)(const int& w,const int& h);
        void (*Render)();
        void (*Tick)(const uint64_t& ms);
    };

    static Overlay *Factory(WinMainArgs &Args,OpenglFunctions &Funcs){
        if (!OverlayOnlyInstance) OverlayOnlyInstance=new Overlay(Args,Funcs);
        return OverlayOnlyInstance;
    }
    ~Overlay(){
        delete OverlayOnlyInstance;
        OverlayOnlyInstance=NULL;
    }

    void Tick(){
        clock _now=now();

        openglFunctions.Tick( toms(_now-lastTime));
        lastTime=_now;
        openglFunctions.Render();
    }

private:
    static Overlay *OverlayOnlyInstance;
    WinMainArgs winMainArgs;
    OpenglFunctions openglFunctions;
    clock lastTime;



    Overlay(WinMainArgs &Args,OpenglFunctions &Funcs){
        winMainArgs=Args;
        openglFunctions=Funcs;

    }
};


#endif
