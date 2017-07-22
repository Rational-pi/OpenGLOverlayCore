#ifndef OPENGL_H
#define OPENGL_H

#include "OpenGL_OverlayWindow.h"

class Overlay:public OpenGL_OverlayWindow
{
public:
    Overlay(HINSTANCE caller):OpenGL_OverlayWindow(caller){}
    void GLInit();
    void GLTick(int64_t milli);
    void GLRender();
    void GLResize(int width,int height);
};


#endif //OPENGL_H
