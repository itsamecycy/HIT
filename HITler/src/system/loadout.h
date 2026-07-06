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
    bool ConsumeEquip();

    void NextSlot();
    void PrevSlot();
    int GetSelectedIndex() const;
    const char* GetDiagnostics() const;

private:
    Gun revolver;
    bool revolverLoaded;
    bool revolverEquipped;
    bool justEquipped;
    // simple weapon list (CS-like slots). Only revolver for now at index 0
    const char* weaponNames[8];
    const char* weaponPaths[8];
    int weaponCount;
    int selectedIndex;
    char diagnostics[512];
};

#endif
