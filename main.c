#include "raylib.h"
#include "rotors.h"
#include "conversions.h"

bool IsAnyKeyPressed() {
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Enigma Sim");

    // left-most rotor is usually rotor 3
    // Enigma I from 1930

    int rotorIII_config[26] = {2, 4, 6, 8, 10, 12, 3, 16, 18, 20, 24, 22, 26, 14, 25, 5, 9, 23, 7, 1, 11, 13, 21, 19, 17, 15};
    Rotor rotorRight = InitialiseRotor(rotorIII_config,22,0);

    int rotorII_config[26] = {1, 10, 4, 11, 19, 9, 18, 21, 24, 2, 12, 8, 23, 20, 18, 3, 17, 7, 26, 14, 25, 6, 22, 15, 5};
    Rotor rotorMiddle = InitialiseRotor(rotorII_config,5,0);

    int rotorI_config[26] = {5, 11, 13, 6, 12, 7, 4, 17, 22, 26, 14, 20, 15, 23, 25, 8, 24, 21, 19, 16, 1, 9, 2, 18, 3, 10};
    Rotor rotorLeft = InitialiseRotor(rotorI_config,17,0);

    while (!WindowShouldClose()) {

        if (IsAnyKeyPressed()) {

            int keyValue = 0;
            int key = GetCharPressed();
            TraceLog(LOG_INFO, "Key Pressed: %c (Ascii: %d)", key, key);

            turn(&rotorRight, &rotorMiddle, &rotorLeft);

            keyValue = toValue(key);
            TraceLog(LOG_INFO, "Right Rotor Input: %d", keyValue);

            int forwardRight = forwardEncipher(&rotorRight, keyValue);
            TraceLog(LOG_INFO, "Middle Rotor Input: %d (%c)", forwardRight, toAscii(forwardRight));

            int forwardMiddle = forwardEncipher(&rotorMiddle, forwardRight);
            TraceLog(LOG_INFO, "Left Rotor Input: %d (%c)", forwardMiddle, toAscii(forwardMiddle));

            int forwardLeft = forwardEncipher(&rotorLeft, forwardMiddle);
            TraceLog(LOG_INFO, "Reflector Input: %d (%c)", forwardLeft, toAscii(forwardLeft));


        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Enigma Single Rotor", 26, 38, 29, LIGHTGRAY);

        EndDrawing();

    }

    CloseWindow();

    return 0;
}