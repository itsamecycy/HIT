#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <string>

enum MenuResult
{
    MENU_NONE = 0,
    MENU_START,
    MENU_LOAD,
    MENU_SETTINGS,
    MENU_CREDITS,
    MENU_EXIT
};

class MainMenu
{
public:
    MainMenu();
    MenuResult Update(int screenWidth, int screenHeight);
    void Draw(int screenWidth, int screenHeight) const;
    void Reset();

private:
    std::string title;
    int selected;
    static constexpr int optionCount = 5;
    std::string options[optionCount];
};

#endif // MAIN_MENU_H