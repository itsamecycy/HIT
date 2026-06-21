#include "raylib.h"
#include <iostream>

using namespace std;

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "HITler");

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 2.0f, 0.0f };
    camera.target = { 0.0f, 2.0f, 1.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor(); // FPS mouse look

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // First-person movement
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        BeginDrawing();

        ClearBackground(GRAY);

        BeginMode3D(camera);

        // World
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f }, LIGHTGRAY);

        DrawCube({ 0.0f, 1.0f, 5.0f }, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires({ 0.0f, 1.0f, 5.0f }, 2.0f, 2.0f, 2.0f, BLACK);

        DrawCube({ 5.0f, 1.0f, 10.0f }, 2.0f, 2.0f, 2.0f, BLUE);
        DrawCubeWires({ 5.0f, 1.0f, 10.0f }, 2.0f, 2.0f, 2.0f, BLACK);

        DrawGrid(100, 2.0f);

        EndMode3D();

        DrawFPS(10, 10);

        DrawText("WASD = Move", 10, 40, 20, BLACK);
        DrawText("Mouse = Look Around", 10, 65, 20, BLACK);

        EndDrawing();
    }

    EnableCursor();
    CloseWindow();

    return 0;
}