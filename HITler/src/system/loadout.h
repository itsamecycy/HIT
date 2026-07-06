#ifndef LOADOUT_H
#define LOADOUT_H

#include "raylib.h"
#include "models/guns.h"

class Loadout
{
public:
    Loadout();
    ~Loadout();

    void Init();
    void DrawMenu(int screenW, int screenH);
    void EquipRevolver();
    Gun* GetEquippedGun();

private:
    Gun revolver;
    bool revolverLoaded;
    bool revolverEquipped;
};

#endif
