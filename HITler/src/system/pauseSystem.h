#ifndef PAUSE_SYSTEM_H
#define PAUSE_SYSTEM_H

enum PauseResult
{
    PAUSE_NONE = 0,
    PAUSE_RESUME,
    PAUSE_SETTINGS,
    PAUSE_MAIN_MENU
};

class PauseSystem
{
public:
    PauseSystem();
    PauseResult Update(int screenWidth, int screenHeight);
    void Draw(int screenWidth, int screenHeight) const;
    void Reset();

private:
    int selected;
    static constexpr int optionCount = 3;
    const char* options[optionCount];
};

#endif // PAUSE_SYSTEM_H
