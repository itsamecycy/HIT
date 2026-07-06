#include "system/loadout.h"
#include <cstdio>
#include <cstring>

Loadout::Loadout()
{
    revolverLoaded = false;
    revolverEquipped = false;
    justEquipped = false;
    weaponCount = 0;
    selectedIndex = 0;
    for (int i = 0; i < 8; ++i)
    {
        weaponNames[i] = "Empty";
        weaponPaths[i] = "";
    }
    diagnostics[0] = '\0';
}

Loadout::~Loadout()
{
}

void Loadout::Init()
{
    // Try several directories and extensions to find a usable revolver model
    const char* bases[] = { "Revolver_1", "Revolver_2", "Revolver_3", "Revolver_4", "Revolver_5" };
    const char* dirs[] = { "assets/models/FBX", "assets/models", "assets/models/guns", "assets/models/FBX/Accessories" };
    const char* exts[] = { ".fbx", ".glb", ".gltf" };

    // prepare diagnostics
    diagnostics[0] = '\0';

    for (int d = 0; d < sizeof(dirs)/sizeof(dirs[0]); ++d)
    {
        for (int b = 0; b < sizeof(bases)/sizeof(bases[0]); ++b)
        {
            for (int e = 0; e < sizeof(exts)/sizeof(exts[0]); ++e)
            {
                char path[256];
                snprintf(path, sizeof(path), "%s/%s%s", dirs[d], bases[b], exts[e]);
                // record whether the file exists
                bool exists = FileExists(path);
                int written = strlen(diagnostics);
                snprintf(diagnostics + written, sizeof(diagnostics) - written, "%s -> %s\n", path, exists ? "exists" : "missing");
                if (revolver.LoadModel(path))
                {
                    // prefer real model over placeholder
                    if (!revolver.IsPlaceholder())
                    {
                        revolverLoaded = true;
                        return;
                    }
                    // record placeholder but keep searching for real model
                    revolverLoaded = true;
                }
            }
        }
    }

    // populate weapon slots (CS-like layout). Only revolver available at slot 0
    weaponNames[0] = "Revolver";
    weaponPaths[0] = "assets/models/FBX/Revolver_1.fbx";
    weaponCount = 1;
}

void Loadout::DrawMenu(int screenW, int screenH)
{
    const char* title = "Loadout";
    int panelW = 700; int panelH = 380;
    int panelX = screenW/2 - panelW/2; int panelY = 80;
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.85f));
    DrawText(title, panelX + 20, panelY + 16, 28, RAYWHITE);

    // Slots row (like CS weapon slots)
    int slotX = panelX + 20;
    int slotY = panelY + 64;
    int slotW = 80; int slotH = 80; int gap = 12;
    for (int i = 0; i < 8; ++i)
    {
        Rectangle r = { (float)slotX + i*(slotW+gap), (float)slotY, (float)slotW, (float)slotH };
        Color bg = (i == selectedIndex) ? DARKGRAY : GRAY;
        DrawRectangleRec(r, bg);
        DrawRectangleLines((int)r.x, (int)r.y, (int)r.width, (int)r.height, BLACK);
        const char* name = (i < weaponCount) ? weaponNames[i] : "Empty";
        DrawText(name, (int)(r.x + r.width/2 - MeasureText(name, 12)/2), (int)(r.y + r.height + 6), 12, RAYWHITE);
    }

    // Preview pane on the right
    int previewX = panelX + panelW - 260; int previewY = panelY + 64; int previewW = 220; int previewH = 220;
    DrawRectangle(previewX, previewY, previewW, previewH, Fade(DARKGRAY, 0.9f));
    DrawRectangleLines(previewX, previewY, previewW, previewH, BLACK);

    // Draw preview model if selected
    if (selectedIndex < weaponCount && selectedIndex == 0 && revolverLoaded)
    {
        // simple preview: draw model near center of preview rect using a small camera
        Camera3D cam = {0};
        cam.position = {0.0f, 0.5f, 2.0f};
        cam.target = {0.0f, 0.5f, 0.0f};
        cam.up = {0.0f, 1.0f, 0.0f};
        cam.fovy = 45.0f;
        BeginScissorMode(previewX, previewY, previewW, previewH);
        BeginMode3D(cam);
        revolver.DrawPreview((Vector3){0.0f, 0.3f, 0.0f}, 0.8f);
        EndMode3D();
        EndScissorMode();
    }

    // Equip button
    Rectangle btn = { (float)(panelX + panelW/2 - 60), (float)(panelY + panelH - 64), 120.0f, 40.0f };
    DrawRectangleRec(btn, DARKBLUE);
    DrawText("Equip", (int)(btn.x + btn.width/2 - MeasureText("Equip", 20)/2), (int)(btn.y + 8), 20, RAYWHITE);

    // Mouse equip
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, btn))
    {
        if (selectedIndex == 0)
        {
            revolverEquipped = true;
            justEquipped = true;
        }
    }

    // Keyboard navigation
    if (IsKeyPressed(KEY_RIGHT)) NextSlot();
    if (IsKeyPressed(KEY_LEFT)) PrevSlot();
    if (IsKeyPressed(KEY_ENTER))
    {
        if (selectedIndex == 0)
        {
            revolverEquipped = true;
            justEquipped = true;
        }
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

bool Loadout::ConsumeEquip()
{
    if (justEquipped)
    {
        justEquipped = false;
        return true;
    }
    return false;
}

const char* Loadout::GetDiagnostics() const
{
    return diagnostics;
}

void Loadout::NextSlot()
{
    selectedIndex = (selectedIndex + 1) % 8;
}

void Loadout::PrevSlot()
{
    selectedIndex = (selectedIndex - 1 + 8) % 8;
}

int Loadout::GetSelectedIndex() const
{
    return selectedIndex;
}
