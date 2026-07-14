#include "settings.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

Settings::Settings()
    : masterVolume(0.7f)
    , currentResolution(0)
    , backgroundColor({135, 206, 235, 255})
    , shouldReturn(false)
    , selectedOption(0)
    , selectedSubOption(0)
    , isInputMode(false)
    , inputIndex(0)
{
    inputBuffer[0] = '\0';
}

void Settings::Update(int screenWidth, int screenHeight)
{
    if (isInputMode)
    {
        // Handle numeric input for RGB value
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 48 && key <= 57) && inputIndex < 3)  // 0-9
            {
                inputBuffer[inputIndex] = (char)key;
                inputIndex++;
                inputBuffer[inputIndex] = '\0';
            }
            key = GetCharPressed();
        }
        
        // Backspace to delete
        if (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0)
        {
            inputIndex--;
            inputBuffer[inputIndex] = '\0';
        }
        
        // Enter to confirm
        if (IsKeyPressed(KEY_ENTER))
        {
            int value = atoi(inputBuffer);
            if (value > 255) value = 255;
            
            if (selectedSubOption == 0)
                backgroundColor.r = (unsigned char)value;
            else if (selectedSubOption == 1)
                backgroundColor.g = (unsigned char)value;
            else if (selectedSubOption == 2)
                backgroundColor.b = (unsigned char)value;
            
            isInputMode = false;
            inputIndex = 0;
            inputBuffer[0] = '\0';
        }
        
        // Escape to cancel
        if (IsKeyPressed(KEY_ESCAPE))
        {
            isInputMode = false;
            inputIndex = 0;
            inputBuffer[0] = '\0';
        }
        
        return;
    }
    
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
        
        // Enter to start input mode
        if (IsKeyPressed(KEY_ENTER))
        {
            isInputMode = true;
            inputIndex = 0;
            inputBuffer[0] = '\0';
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
    
    // If in input mode, show input dialog
    if (isInputMode)
    {
        const char* channels[] = {"Red (R)", "Green (G)", "Blue (B)"};
        const char* channelName = channels[selectedSubOption];
        
        // Draw semi-transparent overlay
        DrawRectangle(200, 250, screenWidth - 400, 200, Fade(BLACK, 0.7f));
        DrawRectangleLines(200, 250, screenWidth - 400, 200, WHITE);
        
        // Draw prompt
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Enter %s value (0-255):", channelName);
        DrawText(prompt, screenWidth / 2 - MeasureText(prompt, 24) / 2, 280, 24, YELLOW);
        
        // Draw input box
        DrawRectangle(screenWidth / 2 - 100, 330, 200, 40, DARKGRAY);
        DrawRectangleLines(screenWidth / 2 - 100, 330, 200, 40, WHITE);
        DrawText(inputBuffer, screenWidth / 2 - MeasureText(inputBuffer, 32) / 2, 340, 32, WHITE);
        
        // Draw hints
        DrawText("Press Enter to confirm, ESC to cancel", screenWidth / 2 - MeasureText("Press Enter to confirm, ESC to cancel", 16) / 2, 400, 16, LIGHTGRAY);
        
        return;
    }
    
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
                    DrawText("Press Enter to input", colorBoxX - 220, colorBoxY + 65, 12, LIGHTGRAY);
                }
            }
        }
    }
    
    if (selectedOption == OPTION_BACKGROUND_COLOR && !isInputMode)
    {
        DrawText("Use Up/Down to navigate, Left/Right to select channel, Enter to input, ESC to return", 
            screenWidth / 2 - MeasureText("Use Up/Down to navigate, Left/Right to select channel, Enter to input, ESC to return", 18) / 2, 
            screenHeight - 50, 18, LIGHTGRAY);
    }
    else
    {
        DrawText("Use Up/Down to navigate, Left/Right to adjust, ESC to return", 
            screenWidth / 2 - MeasureText("Use Up/Down to navigate, Left/Right to adjust, ESC to return", 18) / 2, 
            screenHeight - 50, 18, LIGHTGRAY);
    }
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
