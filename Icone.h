#ifndef ICONE_H
#define ICONE_H

#include "GLObject.h"
#include <string>

class Icone:public GlObject
{
public:
    struct Pose{
        float x;
        float y;
        float scale;
    }pose;
    bool visible;
    GLuint textureID;
    std::string filepath;
    Icone(std::string filepath) {
        this->filepath=filepath;
    }
    void Init();
    void Tick(const int64_t &milliseconds);
    void Render();
};

#endif //ICONE_H
