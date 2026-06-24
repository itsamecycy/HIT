#include "raylib.h"
#include "mainMenu.h"

void MainMenu::Draw()
{
    DrawText(title.c_str(), 10, 10, 40, RED);
}