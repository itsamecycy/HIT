#include "raylib.h"
#include "mainMenu.h"

MainMenu::MainMenu()
    : title("HITler")
    , selected(0)
    , options{"Start", "Load", "Settings", "Credits", "Exit"}
{
}

MenuResult MainMenu::Update(int screenWidth, int screenHeight)
{
    const int optionFontSize = 32;
    const int startY = 240;
    const int spacing = 60;

    Vector2 mousePosition = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int hoverIndex = -1;

    for (int i = 0; i < optionCount; ++i)
    {
        int textWidth = MeasureText(options[i].c_str(), optionFontSize);
        int textX = screenWidth / 2 - textWidth / 2;
        int textY = startY + i * spacing;
        Rectangle optionRect = {(float)textX - 20.0f, (float)textY - 10.0f, (float)textWidth + 40.0f, (float)optionFontSize + 20.0f};

        if (CheckCollisionPointRec(mousePosition, optionRect))
        {
            hoverIndex = i;
            if (clicked)
            {
                return static_cast<MenuResult>(i + 1);
            }
        }
    }

    if (hoverIndex >= 0)
    {
        selected = hoverIndex;
    }
    else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        return static_cast<MenuResult>(selected + 1);
    }

    return MENU_NONE;
}

void MainMenu::Draw(int screenWidth, int screenHeight) const
{
    const int titleFontSize = 64;
    const int titleWidth = MeasureText(title.c_str(), titleFontSize);
    DrawText(title.c_str(), screenWidth / 2 - titleWidth / 2, 100, titleFontSize, RED);

    const int optionFontSize = 32;
    const int startY = 240;
    const int spacing = 60;

    for (int i = 0; i < optionCount; ++i)
    {
        const std::string& option = options[i];
        int textWidth = MeasureText(option.c_str(), optionFontSize);
        int textX = screenWidth / 2 - textWidth / 2;
        int textY = startY + i * spacing;
        Rectangle optionRect = {(float)textX - 20.0f, (float)textY - 10.0f, (float)textWidth + 40.0f, (float)optionFontSize + 20.0f};

        if (i == selected)
        {
            DrawRectangleRec(optionRect, Fade(LIGHTGRAY, 0.6f));
            DrawText(option.c_str(), textX, textY, optionFontSize, BLACK);
        }
        else
        {
            DrawText(option.c_str(), textX, textY, optionFontSize, WHITE);
        }
    }

    const char* hint = "Click an option to choose.";
    DrawText(hint, screenWidth / 2 - MeasureText(hint, 20) / 2, screenHeight - 60, 20, LIGHTGRAY);
}

void MainMenu::Reset()
{
    selected = 0;
}
