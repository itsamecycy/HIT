#include "models/guns.h"
#include "raymath.h"

Gun::Gun()
{
    loaded = false;
    model = {0};
}

Gun::~Gun()
{
    Unload();
}

bool Gun::LoadModel(const char* modelPath)
{
    if (loaded) Unload();
    model = ::LoadModel(modelPath);
    // If model meshes were scaled/rotated in authoring, we might need to adjust here
    loaded = true;
    return loaded;
}

void Gun::DrawInHand(const Camera3D& camera, const Vector3& handOffset)
{
    if (!loaded) return;

    // Compute a simple right-hand transform relative to camera
    Vector3 handPos = Vector3Add(camera.position, handOffset);
    DrawModel(model, handPos, 1.0f, WHITE);
}

void Gun::Unload()
{
    if (loaded)
    {
        UnloadModel(model);
        loaded = false;
    }
}
