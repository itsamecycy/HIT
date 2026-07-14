#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"

class Settings
{
public:
    Settings();
    
    void Update(int screenWidth, int screenHeight);
    void Draw(int screenWidth, int screenHeight) const;
    void Reset();
    
    bool ShouldReturn() const { return shouldReturn; }
    
    // Getters for settings values
    float GetMasterVolume() const { return masterVolume; }
    int GetResolutionWidth() const { return resolutionPresets[currentResolution][0]; }
    int GetResolutionHeight() const { return resolutionPresets[currentResolution][1]; }
    Color GetBackgroundColor() const { return backgroundColor; }
    
    // For applying settings
    void SetMasterVolume(float vol) { masterVolume = vol; }
    void ApplyResolution(int& screenWidth, int& screenHeight);
    
private:
    float masterVolume;
    int currentResolution;
    Color backgroundColor;
    bool shouldReturn;
    
    int selectedOption;
    int selectedSubOption;
    
    // Resolution presets: width, height
    static constexpr int resolutionCount = 4;
    static constexpr int resolutionPresets[4][2] = {
        {1280, 720},
        {1920, 1080},
        {1024, 768},
        {800, 600}
    };
    static constexpr const char* resolutionLabels[4] = {
        "1280x720", "1920x1080", "1024x768", "800x600"
    };
    
    enum SettingOption
    {
        OPTION_AUDIO = 0,
        OPTION_RESOLUTION = 1,
        OPTION_BACKGROUND_COLOR = 2,
        OPTION_BACK = 3,
        OPTION_COUNT = 4
    };
    
    const char* optionNames[OPTION_COUNT] = {
        "Audio Volume",
        "Resolution",
        "Background Color",
        "Back"
    };
};

#endif // SETTINGS_H
