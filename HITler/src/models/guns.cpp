#include "models/guns.h"
#include "raymath.h"
#include <cstdio>

Gun::Gun()
{
    loaded = false;
    model = {0};
    placeholder = false;
    message[0] = '\0';
}

Gun::~Gun()
{
    Unload();
}

bool Gun::LoadModel(const char* modelPath)
{
    if (loaded) Unload();
    model = ::LoadModel(modelPath);
    // consider a model loaded only if it contains at least one mesh
    if (model.meshCount > 0)
    {
        loaded = true;
        placeholder = false;
        snprintf(message, sizeof(message), "Loaded model: %s (meshes=%d)", modelPath, model.meshCount);
        return true;
    }

    // model failed to load or is empty
    loaded = false;
    // unload any partially loaded data to avoid leaks
    UnloadModel(model);
    // create a placeholder model so player can still see a gun-like object
    Mesh m = GenMeshCylinder(0.05f, 0.02f, 6);
    model = LoadModelFromMesh(m);
    loaded = true;
    placeholder = true;
    snprintf(message, sizeof(message), "Failed to load %s - using placeholder", modelPath);
    return true;
}

void Gun::DrawInHand(const Camera3D& camera, const Vector3& handOffset)
{
    if (!loaded) return;

    // Compute a simple right-hand transform relative to camera
    // Compute a right-offset and forward offset so model appears on the right-hand
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

    Vector3 handPos = Vector3Add(camera.position, Vector3Scale(forward, handOffset.z));
    handPos = Vector3Add(handPos, Vector3Scale(right, handOffset.x));
    handPos = Vector3Add(handPos, Vector3Scale(camera.up, handOffset.y));

    // Draw with a small scale and slight rotation so most FBX models are visible
    float scale = 0.8f;
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    float rotationAngle = 10.0f; // degrees
    DrawModelEx(model, handPos, rotationAxis, rotationAngle, (Vector3){scale, scale, scale}, WHITE);
}

bool Gun::IsPlaceholder() const
{
    return placeholder;
}

const char* Gun::LoadMessage() const
{
    return message;
}

void Gun::DrawPreview(const Vector3& position, float scale) const
{
    if (!loaded) return;
    DrawModelEx(model, position, (Vector3){0.0f,1.0f,0.0f}, 0.0f, (Vector3){scale,scale,scale}, WHITE);
}

int Gun::GetMeshCount() const
{
    return model.meshCount;
}

void Gun::Unload()
{
    if (loaded)
    {
        UnloadModel(model);
        loaded = false;
    }
}
