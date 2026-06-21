#include "testmap.h"

void TestMap::Draw()
{
    DrawPlane({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f }, LIGHTGRAY);

    DrawCube({ 0.0f, 1.0f, 5.0f }, 2.0f, 2.0f, 2.0f, RED);
    DrawCubeWires({ 0.0f, 1.0f, 5.0f }, 2.0f, 2.0f, 2.0f, BLACK);

    DrawCube({ 5.0f, 1.0f, 10.0f }, 2.0f, 2.0f, 2.0f, BLUE);
    DrawCubeWires({ 5.0f, 1.0f, 10.0f }, 2.0f, 2.0f, 2.0f, BLACK);

    DrawGrid(100, 1.0f);
}