#include "system/loadout.h"

Loadout::Loadout()
{
    revolverLoaded = false;
    revolverEquipped = false;
}

Loadout::~Loadout()
{
}

void Loadout::Init()
{
    // Try several possible paths under assets/models/FBX
    const char* candidates[] = {
        "assets/models/FBX/Revolver_1.fbx",
        "assets/models/FBX/Revolver_2.fbx",
        "assets/models/FBX/Revolver_3.fbx",
        "assets/models/FBX/Revolver_4.fbx",
        "assets/models/FBX/Revolver_5.fbx"
    };

    for (int i = 0; i < sizeof(candidates)/sizeof(candidates[0]); ++i)
    {
        if (revolver.LoadModel(candidates[i]))
        {
            revolverLoaded = true;
            break;
        }
    }
}

void Loadout::DrawMenu(int screenW, int screenH)
{
    // Simple UI: show Revolver slot and equip button
    const char* title = "Loadout";
    DrawRectangle(screenW/2 - 200, 80, 400, 240, Fade(BLACK, 0.7f));
    DrawText(title, screenW/2 - MeasureText(title, 36)/2, 92, 36, RAYWHITE);

    const char* slot = "Revolver";
    DrawText(slot, screenW/2 - MeasureText(slot, 24)/2, 150, 24, RAYWHITE);

    const Rectangle btn = { (float)(screenW/2 - 60), 190.0f, 120.0f, 40.0f };
    DrawRectangleRec(btn, DARKGRAY);
    DrawText("Equip", (int)(btn.x + btn.width/2 - MeasureText("Equip", 20)/2), (int)(btn.y + 8), 20, RAYWHITE);

    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, btn))
    {
        revolverEquipped = true;
    }
}

void Loadout::EquipRevolver()
{
    revolverEquipped = true;
}

Gun* Loadout::GetEquippedGun()
{
    if (revolverEquipped && revolverLoaded) return &revolver;
    return nullptr;
}
