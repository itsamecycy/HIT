#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <string>

class MainMenu
{
    public:
        MainMenu();
        void Draw();
        void Update();
        void Reset();

    private:
        std::string title = "HITler";
        int selected = 0;
        int optionCount = 2;
        bool start = false;
        bool quit = false;
        bool reset = false;

        std::string options[4] = {"Start", "Load","Settings", "Quit"};

};

#endif // MAIN_MENU_H