#ifndef GUNS_H
#define GUNS_H

#include "raylib.h"

class Gun
{
public:
    Gun();
    ~Gun();

    bool LoadModel(const char* modelPath);
    void DrawInHand(const Camera3D& camera, const Vector3& handOffset);
    void Unload();

private:
    Model model;
    bool loaded;
};

#endif
