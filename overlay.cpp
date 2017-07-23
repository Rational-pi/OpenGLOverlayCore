#include "overlay.h"
#include <GL/gl.h>
#include <GL/glu.h>

void Overlay::Run()
{
    while (!killme) {
        Tick();
    }
}

void Overlay::GLInit(){
    glEnable(GL_TEXTURE_2D);
    //end new

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}

void Overlay::GLRender(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    glColor3f(0, 1, 1);
    glBegin(GL_TRIANGLES);                              // Drawing Using Triangles
        glColor3f(1.0f,0.0f,0.0f);                      // Set The Color To Red
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top
        glColor3f(0.0f,1.0f,0.0f);                      // Set The Color To Green
        glVertex3f(-1.0f,-1.0f, 0.0f);                  // Bottom Left
        glColor3f(0.0f,0.0f,1.0f);                      // Set The Color To Blue
        glVertex3f( 1.0f,-1.0f, 0.0f);                  // Bottom Right
    glEnd();


    glFlush();
}

void Overlay::GLResize(int w, int h) {
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
