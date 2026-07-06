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
    bool IsPlaceholder() const;
    const char* LoadMessage() const;
    void DrawPreview(const Vector3& position, float scale) const;
    int GetMeshCount() const;

private:
    Model model;
    bool loaded;
    bool placeholder;
    char message[128];
};

#endif
