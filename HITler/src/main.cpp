#include "raylib.h"
#include "system/playerMovement.h"
#include "system/pauseSystem.h"
#include "scenes/mainMenu.h"
#include "raymath.h"
#include "system/loadout.h"
#include <float.h>

enum class GameState
{
    MAIN_MENU,
    MENU_SETTINGS,
    MENU_CREDITS,
    PLAYING,
    PAUSED
};

int main()
{
    Color backgroundColor = { 135, 206, 235, 200 };
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "HITler");
    SetExitKey(KEY_NULL);
    EnableCursor();
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = {0, 2, 0};
    camera.target = {0, 2, 1};
    camera.up = {0, 1, 0};
    camera.fovy = 60;
    camera.projection = CAMERA_PERSPECTIVE;

    InitAudioDevice();
    Music mainMenuMusic = LoadMusicStream("assets/sounds/bgm/mainMenu_bgm.mp3");
    mainMenuMusic.looping = true;
    SetMusicVolume(mainMenuMusic, 0.5f);

    Sound walkSound = LoadSound("assets/sounds/sfx/walking_footsteps.mp3");
    Sound sprintSound = LoadSound("assets/sounds/sfx/running_sfx.mp3");
    SetSoundVolume(walkSound, 0.7f);
    SetSoundVolume(sprintSound, 0.7f);

    PlayerMovement player;
    MainMenu mainMenu;
    PauseSystem pauseSystem;
    Loadout loadout;
    bool showLoadout = false;
    GameState currentState = GameState::MAIN_MENU;
    bool menuMusicActive = false;

    Obstacle obstacles[] =
    {
        { { { -10.0f, 0.0f, -10.0f }, { -8.0f, 18.0f, -8.0f } }, GRAY },
        { { { 4.0f, 0.0f, -12.0f }, { 6.0f, 24.0f, -10.0f } }, DARKGRAY },
        { { { -2.0f, 0.0f, 4.0f }, { 0.0f, 16.0f, 6.0f } }, LIGHTGRAY },
        { { { 8.0f, 0.0f, 2.0f }, { 10.0f, 20.0f, 4.0f } }, GRAY }
    };
    const int obstacleCount = sizeof(obstacles) / sizeof(obstacles[0]);

    // initialize loadout and sounds
    loadout.Init();
    Sound hookSound = LoadSound("assets/sounds/sfx/hook_sfx.mp3");

    while (!WindowShouldClose())
    {
        if (currentState == GameState::PLAYING)
        {
            if (!showLoadout)
            {
                player.Update(camera, obstacles, obstacleCount);
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = GameState::PAUSED;
                pauseSystem.Reset();
                EnableCursor();
            }

            if (IsKeyPressed(KEY_R))
            {
                if (player.IsGrappling())
                {
                    player.StopGrapple();
                }
                else if (!showLoadout)
                {
                    Ray ray = {camera.position, Vector3Normalize(Vector3Subtract(camera.target, camera.position))};
                    float bestDistance = FLT_MAX;
                    Vector3 hitPoint = {0};
                    bool hit = false;

                    for (int i = 0; i < obstacleCount; ++i)
                    {
                        RayCollision collision = GetRayCollisionBox(ray, obstacles[i].box);
                        if (collision.hit && collision.distance > 0 && collision.distance < bestDistance)
                        {
                            bestDistance = collision.distance;
                            hitPoint = collision.point;
                            hit = true;
                        }
                    }

                    if (hit)
                    {
                        player.GrappleTo(hitPoint);
                        PlaySound(hookSound);
                    }
                }
            }

            // movement SFX (only when not showing loadout)
            if (!showLoadout)
            {
                if (player.IsWalking())
                {
                    if (!IsSoundPlaying(walkSound))
                    {
                        StopSound(sprintSound);
                        PlaySound(walkSound);
                    }
                }
                else if (player.IsSprinting())
                {
                    if (!IsSoundPlaying(sprintSound))
                    {
                        StopSound(walkSound);
                        PlaySound(sprintSound);
                    }
                }
                else
                {
                    StopSound(walkSound);
                    StopSound(sprintSound);
                }
            }
            else
            {
                StopSound(walkSound);
                StopSound(sprintSound);
            }
        }
        else if (currentState == GameState::PAUSED)
        {
            PauseResult pauseResult = pauseSystem.Update(screenWidth, screenHeight);
            if (pauseResult == PAUSE_RESUME)
            {
                currentState = GameState::PLAYING;
                DisableCursor();
            }
            else if (pauseResult == PAUSE_SETTINGS)
            {
                currentState = GameState::MENU_SETTINGS;
            }
            else if (pauseResult == PAUSE_MAIN_MENU)
            {
                currentState = GameState::MAIN_MENU;
                mainMenu.Reset();
                EnableCursor();
                player = PlayerMovement();
            }
        }
        else if (currentState == GameState::MAIN_MENU)
        {
            MenuResult result = mainMenu.Update(screenWidth, screenHeight);
            if (result == MENU_START || result == MENU_LOAD)
            {
                currentState = GameState::PLAYING;
                DisableCursor();
            }
            else if (result == MENU_SETTINGS)
            {
                currentState = GameState::MENU_SETTINGS;
            }
            else if (result == MENU_CREDITS)
            {
                currentState = GameState::MENU_CREDITS;
            }
            else if (result == MENU_EXIT)
            {
                break;
            }
        }
        else if (currentState == GameState::MENU_SETTINGS || currentState == GameState::MENU_CREDITS)
        {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE))
            {
                currentState = GameState::MAIN_MENU;
            }
        }

        if (currentState == GameState::MAIN_MENU)
        {
            if (!menuMusicActive)
            {
                PlayMusicStream(mainMenuMusic);
                menuMusicActive = true;
            }
        }
        else
        {
            if (menuMusicActive)
            {
                StopMusicStream(mainMenuMusic);
                menuMusicActive = false;
            }
        }

        if (menuMusicActive)
        {
            UpdateMusicStream(mainMenuMusic);
        }

        // Toggle loadout menu (now bound to B)
        if (IsKeyPressed(KEY_B))
        {
            showLoadout = !showLoadout;
            if (showLoadout) EnableCursor(); else DisableCursor();
        }

        BeginDrawing();

        if (currentState == GameState::MAIN_MENU)
        {
            ClearBackground(BLACK);
            mainMenu.Draw(screenWidth, screenHeight);
        }
        else if (currentState == GameState::MENU_SETTINGS)
        {
            ClearBackground(GRAY);
            const char* title = "Settings";
            DrawText(title, screenWidth / 2 - MeasureText(title, 48) / 2, 120, 48, RED);
            DrawText("Audio, graphics and controls are not implemented yet.", screenWidth / 2 - MeasureText("Audio, graphics and controls are not implemented yet.", 24) / 2, 240, 24, WHITE);
            DrawText("Press ESC or Enter to return.", screenWidth / 2 - MeasureText("Press ESC or Enter to return.", 20) / 2, screenHeight - 80, 20, LIGHTGRAY);
        }
        else if (currentState == GameState::MENU_CREDITS)
        {
            ClearBackground(GRAY);
            const char* title = "Credits";
            DrawText(title, screenWidth / 2 - MeasureText(title, 48) / 2, 120, 48, RED);
            DrawText("Game created by HITler team.", screenWidth / 2 - MeasureText("Game created by HITler team.", 24) / 2, 240, 24, WHITE);
            DrawText("Press ESC or Enter to return.", screenWidth / 2 - MeasureText("Press ESC or Enter to return.", 20) / 2, screenHeight - 80, 20, LIGHTGRAY);
        }
        else if (currentState == GameState::PLAYING || currentState == GameState::PAUSED)
        {
            ClearBackground(GRAY);
            BeginMode3D(camera);
            DrawGrid(50, 2.0f);
            // debug cubes removed

            Vector3 rampA = {6.0f, 0.0f, -6.0f};
            Vector3 rampB = {9.0f, 0.0f, -6.0f};
            Vector3 rampC = {6.0f, 2.0f, -4.0f};
            DrawTriangle3D(rampA, rampB, rampC, ORANGE);
            DrawTriangle3D(rampB, {9.0f, 0.0f, -4.0f}, rampC, BROWN);

            // more debug cubes removed

            for (int i = 0; i < obstacleCount; ++i)
            {
                BoundingBox box = obstacles[i].box;
                Vector3 size = { box.max.x - box.min.x, box.max.y - box.min.y, box.max.z - box.min.z };
                Vector3 center = { box.min.x + size.x / 2.0f, box.min.y + size.y / 2.0f, box.min.z + size.z / 2.0f };
                DrawCube(center, size.x, size.y, size.z, obstacles[i].color);
                DrawCubeWires(center, size.x, size.y, size.z, BLACK);
            }

            if (player.IsGrappling())
            {
                DrawLine3D(camera.position, player.GetGrappleTarget(), YELLOW);
                DrawSphere(player.GetGrappleTarget(), 0.15f, GOLD);
            }

            EndMode3D();

            // draw equipped gun in right hand
            Gun* equipped = loadout.GetEquippedGun();
            if (equipped && !showLoadout)
            {
                equipped->DrawInHand(camera, {0.4f, -0.2f, 0.6f});
            }

            // draw loadout UI overlay if open
            if (showLoadout)
            {
                loadout.DrawMenu(screenWidth, screenHeight);
            }

            if (currentState == GameState::PAUSED)
            {
                pauseSystem.Draw(screenWidth, screenHeight);
            }
        }

        if (currentState == GameState::PLAYING)
        {
            DrawCircle(screenWidth / 2, screenHeight / 2, 3, WHITE);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    StopMusicStream(mainMenuMusic);
    UnloadMusicStream(mainMenuMusic);
    UnloadSound(walkSound);
    UnloadSound(sprintSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
