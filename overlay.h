#ifndef OPENGL_H
#define OPENGL_H
#include "OpenGL_OverlayWindow.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "3rd/soil/SOIL.h"

#include "Icone.h"


class Overlay:public OpenGL_OverlayWindow{
    Icone *icoTest;
public:
    Overlay(HINSTANCE caller,std::string WindowName, Rect rect):OpenGL_OverlayWindow(caller,WindowName,rect){}
protected:
    void GLInit();
    void GLTick(int64_t milli){}
    void GLRender();
    void GLResize(int w, int h);
};

#endif //OPENGL_H
