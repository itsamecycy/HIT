#include "raylib.h"
#include "system/playerMovement.h"
#include "raymath.h"

int main()
{
    InitWindow(1280, 720, "HITler");

    DisableCursor();
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = {0, 2, 0};
    camera.target = {0, 2, 1};
    camera.up = {0, 1, 0};
    camera.fovy = 60;
    camera.projection = CAMERA_PERSPECTIVE;

    PlayerMovement player;

    while (!WindowShouldClose())
    {
        player.Update(camera);

        BeginDrawing();
        ClearBackground(GRAY);

        BeginMode3D(camera);

        DrawGrid(50, 1.0f);
        DrawCube({0,1,5}, 2,2,2, RED);

        EndMode3D();

        DrawFPS(10,10);
        EndDrawing();
    }

    CloseWindow();
}