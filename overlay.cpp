#include "overlay.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <ctime> // sleep()


void Overlay::GLInit(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0, 0, 0, 0);


    icoTest= new Icone("C:/Users/lvl-z/Desktop/PlayStation_1_Logo.png");
    icoTest->Init();


}
void Overlay::GLRender(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    icoTest->Render();


    {
        /*  check for GL errors    */
        GLenum err_code = glGetError();
        while( GL_NO_ERROR != err_code )
        {
            printf( "OpenGL Error @ %s: %i", "drawing loop", err_code );
            err_code = glGetError();
        }
    }
    //end new
    glFlush();
}
void Overlay::GLResize(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

