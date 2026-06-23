#include "raylib.h"
#include "system/playerMovement.h"
#include "raymath.h"
#include <iostream>

int main()
{
    // Window configuration
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "HITler");

    DisableCursor();
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = {0, 2, 0};
    camera.target = {0, 2, 1};
    camera.up = {0, 1, 0};
    camera.fovy = 60;
    camera.projection = CAMERA_PERSPECTIVE;

    PlayerMovement player;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        player.Update(camera);

        // Rendering
        BeginDrawing();
        ClearBackground(GRAY);

        BeginMode3D(camera);

        DrawGrid(50, 2.0f);
        DrawCube({0.0f, 1.0f, 5.0f}, 2.0f, 2.0f, 2.0f, RED);

        EndMode3D();

        DrawFPS(10,10);
        EndDrawing();
    }

    CloseWindow();
}