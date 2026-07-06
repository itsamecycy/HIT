#include "raylib.h"
#include "system/playerMovement.h"
#include "system/pauseSystem.h"
#include "scenes/mainMenu.h"
#include "raymath.h"

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
    GameState currentState = GameState::MAIN_MENU;
    bool menuMusicActive = false;

    while (!WindowShouldClose())
    {
        if (currentState == GameState::PLAYING)
        {
            player.Update(camera);

            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = GameState::PAUSED;
                pauseSystem.Reset();
                EnableCursor();
            }

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
            DrawCube({0.0f, 1.0f, 5.0f}, 2.0f, 2.0f, 2.0f, RED);
            DrawCube({-4.0f, 1.0f, 2.0f}, 2.0f, 2.0f, 2.0f, BLUE);
            DrawCube({3.0f, 1.0f, -2.0f}, 2.0f, 2.0f, 2.0f, DARKBLUE);

            Vector3 rampA = {6.0f, 0.0f, -6.0f};
            Vector3 rampB = {9.0f, 0.0f, -6.0f};
            Vector3 rampC = {6.0f, 2.0f, -4.0f};
            DrawTriangle3D(rampA, rampB, rampC, ORANGE);
            DrawTriangle3D(rampB, {9.0f, 0.0f, -4.0f}, rampC, BROWN);

            DrawCube({-8.0f, 1.0f, 4.0f}, 2.0f, 2.0f, 2.0f, SKYBLUE);
            DrawCube({2.0f, 1.0f, 7.0f}, 2.0f, 2.0f, 2.0f, LIME);

            EndMode3D();

            if (currentState == GameState::PAUSED)
            {
                pauseSystem.Draw(screenWidth, screenHeight);
            }
        }

        if (currentState == GameState::PLAYING)
        {
            DrawCircle(screenWidth / 2, screenHeight / 2, 4, WHITE);
            DrawCircleLines(screenWidth / 2, screenHeight / 2, 6, BLACK);
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
