#include "raylib.h"

using namespace std;

int main()
{
    InitWindow(1280, 720, "HITler");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        //Event handling

        //Update

        //Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 10, 10, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}