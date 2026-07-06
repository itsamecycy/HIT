#include "raylib.h"
#include "pauseSystem.h"

PauseSystem::PauseSystem()
    : selected(0)
    , options{"Resume", "Settings", "Main Menu"}
{
}

PauseResult PauseSystem::Update(int screenWidth, int screenHeight)
{
    const int optionFontSize = 32;
    const int startY = screenHeight / 2 - 40;
    const int spacing = 50;

    Vector2 mousePosition = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int hoverIndex = -1;

    for (int i = 0; i < optionCount; ++i)
    {
        int textWidth = MeasureText(options[i], optionFontSize);
        int textX = screenWidth / 2 - textWidth / 2;
        int textY = startY + i * spacing;
        Rectangle optionRect = {(float)textX - 20.0f, (float)textY - 8.0f, (float)textWidth + 40.0f, (float)optionFontSize + 20.0f};

        if (CheckCollisionPointRec(mousePosition, optionRect))
        {
            hoverIndex = i;
            if (clicked)
            {
                if (i == 0)
                    return PAUSE_RESUME;
                if (i == 1)
                    return PAUSE_SETTINGS;
                if (i == 2)
                    return PAUSE_MAIN_MENU;
            }
        }
    }

    if (hoverIndex >= 0)
    {
        selected = hoverIndex;
    }
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        return PAUSE_RESUME;
    }
    else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (selected == 0)
            return PAUSE_RESUME;
        if (selected == 1)
            return PAUSE_SETTINGS;
        if (selected == 2)
            return PAUSE_MAIN_MENU;
    }

    return PAUSE_NONE;
}

void PauseSystem::Draw(int screenWidth, int screenHeight) const
{
    const int overlayX = screenWidth / 2 - 240;
    const int overlayY = screenHeight / 2 - 140;
    const int overlayW = 480;
    const int overlayH = 280;

    DrawRectangle(overlayX, overlayY, overlayW, overlayH, Fade(BLACK, 0.6f));
    DrawText("Paused", screenWidth / 2 - MeasureText("Paused", 48) / 2, overlayY + 20, 48, RAYWHITE);

    for (int i = 0; i < optionCount; ++i)
    {
        int textWidth = MeasureText(options[i], 32);
        int textX = screenWidth / 2 - textWidth / 2;
        int textY = overlayY + 100 + i * 50;

        if (selected == i)
        {
            DrawRectangle(textX - 20, textY - 8, textWidth + 40, 40, Fade(LIGHTGRAY, 0.7f));
            DrawText(options[i], textX, textY, 32, BLACK);
        }
        else
        {
            DrawText(options[i], textX, textY, 32, RAYWHITE);
        }
    }

    DrawText("Use arrow keys and Enter to choose.", screenWidth / 2 - MeasureText("Use arrow keys and Enter to choose.", 20) / 2, overlayY + overlayH - 40, 20, LIGHTGRAY);
}

void PauseSystem::Reset()
{
    selected = 0;
}
