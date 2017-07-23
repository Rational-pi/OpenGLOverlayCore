#ifndef GLOBJECT_H
#define GLOBJECT_H
#include <inttypes.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "3rd/soil/SOIL.h"
class GlObject
{
public:
    virtual void Init()=0;
    virtual void Tick(const int64_t &milliseconds)=0;
    virtual void Render()=0;
};


#endif //GLOBJECT_H
