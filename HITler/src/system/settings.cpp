#include "settings.h"
#include <cstring>
#include <cstdio>

Settings::Settings()
    : masterVolume(0.7f)
    , currentResolution(0)
    , backgroundColor({135, 206, 235, 255})
    , shouldReturn(false)
    , selectedOption(0)
    , selectedSubOption(0)
{
}

void Settings::Update(int screenWidth, int screenHeight)
{
    if (IsKeyPressed(KEY_UP))
    {
        selectedOption--;
        if (selectedOption < 0) selectedOption = OPTION_COUNT - 1;
        selectedSubOption = 0;
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        selectedOption++;
        if (selectedOption >= OPTION_COUNT) selectedOption = 0;
        selectedSubOption = 0;
    }
    
    // Handle sub-option navigation based on current option
    if (selectedOption == OPTION_AUDIO)
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            masterVolume -= 0.1f;
            if (masterVolume < 0.0f) masterVolume = 0.0f;
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            masterVolume += 0.1f;
            if (masterVolume > 1.0f) masterVolume = 1.0f;
        }
    }
    else if (selectedOption == OPTION_RESOLUTION)
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            currentResolution--;
            if (currentResolution < 0) currentResolution = resolutionCount - 1;
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            currentResolution++;
            if (currentResolution >= resolutionCount) currentResolution = 0;
        }
    }
    else if (selectedOption == OPTION_BACKGROUND_COLOR)
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            selectedSubOption--;
            if (selectedSubOption < 0) selectedSubOption = 2;
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            selectedSubOption++;
            if (selectedSubOption > 2) selectedSubOption = 0;
        }
        
        // Adjust RGB values (0 = R, 1 = G, 2 = B)
        if (IsKeyPressed(KEY_UP))
        {
            if (selectedSubOption == 0)
            {
                backgroundColor.r = (unsigned char)((int)backgroundColor.r + 10 > 255 ? 255 : (int)backgroundColor.r + 10);
            }
            else if (selectedSubOption == 1)
            {
                backgroundColor.g = (unsigned char)((int)backgroundColor.g + 10 > 255 ? 255 : (int)backgroundColor.g + 10);
            }
            else if (selectedSubOption == 2)
            {
                backgroundColor.b = (unsigned char)((int)backgroundColor.b + 10 > 255 ? 255 : (int)backgroundColor.b + 10);
            }
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            if (selectedSubOption == 0)
            {
                backgroundColor.r = (unsigned char)((int)backgroundColor.r - 10 < 0 ? 0 : (int)backgroundColor.r - 10);
            }
            else if (selectedSubOption == 1)
            {
                backgroundColor.g = (unsigned char)((int)backgroundColor.g - 10 < 0 ? 0 : (int)backgroundColor.g - 10);
            }
            else if (selectedSubOption == 2)
            {
                backgroundColor.b = (unsigned char)((int)backgroundColor.b - 10 < 0 ? 0 : (int)backgroundColor.b - 10);
            }
        }
    }
    
    // Handle Enter/Escape to go back
    if (IsKeyPressed(KEY_ENTER) && selectedOption == OPTION_BACK)
    {
        shouldReturn = true;
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        shouldReturn = true;
    }
}

void Settings::Draw(int screenWidth, int screenHeight) const
{
    ClearBackground(GRAY);
    
    const char* title = "Settings";
    DrawText(title, screenWidth / 2 - MeasureText(title, 48) / 2, 60, 48, RED);
    
    const int startY = 140;
    const int spacing = 70;
    const int optionFontSize = 28;
    
    // Draw each option
    for (int i = 0; i < OPTION_COUNT; ++i)
    {
        int y = startY + i * spacing;
        bool isSelected = (i == selectedOption);
        
        if (isSelected)
        {
            DrawRectangle(50, y - 10, screenWidth - 100, 50, Fade(LIGHTGRAY, 0.7f));
            DrawText(optionNames[i], 80, y, optionFontSize, BLACK);
        }
        else
        {
            DrawText(optionNames[i], 80, y, optionFontSize, WHITE);
        }
        
        // Draw option values
        if (isSelected || i < OPTION_COUNT - 1)
        {
            if (i == OPTION_AUDIO)
            {
                char volumeText[32];
                snprintf(volumeText, sizeof(volumeText), "%.0f%%", masterVolume * 100.0f);
                int textX = screenWidth - 150;
                DrawText(volumeText, textX, y, optionFontSize, isSelected ? BLACK : YELLOW);
                DrawRectangle(textX - 120, y + 5, 100, 20, DARKGRAY);
                DrawRectangle(textX - 120, y + 5, (int)(masterVolume * 100.0f), 20, GREEN);
            }
            else if (i == OPTION_RESOLUTION)
            {
                int textX = screenWidth - 250;
                DrawText(resolutionLabels[currentResolution], textX, y, optionFontSize, isSelected ? BLACK : YELLOW);
            }
            else if (i == OPTION_BACKGROUND_COLOR)
            {
                // Draw RGB color indicator
                int colorBoxX = screenWidth - 180;
                int colorBoxY = y - 5;
                DrawRectangle(colorBoxX, colorBoxY, 40, 40, backgroundColor);
                DrawRectangleLines(colorBoxX, colorBoxY, 40, 40, WHITE);
                
                // Draw RGB values
                char colorText[64];
                snprintf(colorText, sizeof(colorText), "R:%d G:%d B:%d", 
                    backgroundColor.r, backgroundColor.g, backgroundColor.b);
                DrawText(colorText, colorBoxX - 150, y + 5, 16, isSelected ? BLACK : YELLOW);
                
                // Show which channel is selected when in color mode
                if (isSelected)
                {
                    const char* channels[] = {"[R]", "[G]", "[B]"};
                    DrawText(channels[selectedSubOption], colorBoxX - 160, colorBoxY + 45, 14, ORANGE);
                }
            }
        }
    }
    
    DrawText("Use Up/Down to navigate, Left/Right to adjust, ESC to return", 
        screenWidth / 2 - MeasureText("Use Up/Down to navigate, Left/Right to adjust, ESC to return", 18) / 2, 
        screenHeight - 50, 18, LIGHTGRAY);
}

void Settings::Reset()
{
    shouldReturn = false;
    selectedOption = 0;
    selectedSubOption = 0;
}

void Settings::ApplyResolution(int& screenWidth, int& screenHeight)
{
    screenWidth = resolutionPresets[currentResolution][0];
    screenHeight = resolutionPresets[currentResolution][1];
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
}
