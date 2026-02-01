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

    int rotorII_config[26] = {1, 10, 4, 11, 19, 9, 18, 21, 24, 2, 12, 8, 23, 20, 13, 3, 17, 7, 26, 14, 25, 6, 22, 15, 5};
    Rotor rotorMiddle = InitialiseRotor(rotorII_config,5,0);

    int rotorI_config[26] = {5, 11, 13, 6, 12, 7, 4, 17, 22, 26, 14, 20, 15, 23, 25, 8, 24, 21, 19, 16, 1, 9, 2, 18, 3, 10};
    Rotor rotorLeft = InitialiseRotor(rotorI_config,17,0);

    // just initialising the reflector and plugboard as a (technically stationary) rotor since it doens't seem worth creating a separate struct

    int ukwb_config[26] = {25, 18, 21, 8, 17, 19, 12, 4, 16, 24, 14, 7, 15, 11, 13, 9, 5, 2, 6, 26, 3, 23, 22, 10, 1, 20};
    Rotor reflector = InitialiseRotor(ukwb_config, -1, 0);

    Rotor plugboard_config[26] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
    Rotor plugboard = InitialiseRotor(plugboard_config, -1, 0);

    while (!WindowShouldClose()) {

        if (IsAnyKeyPressed()) {

            int keyValue = 0;
            int key = GetCharPressed();
            TraceLog(LOG_INFO, "Key Pressed: %c (Ascii: %d)", key, key);

            turn(&rotorRight, &rotorMiddle, &rotorLeft);

            keyValue = toValue(key);
            TraceLog(LOG_INFO, "Plugboard Input: %d", keyValue);

            int forwardPlugboard = forwardEncipher(&plugboard, keyValue);
            TraceLog(LOG_INFO, "Right Rotor Input: %d (%c)", forwardPlugboard, toAscii(forwardPlugboard));

            int forwardRight = forwardEncipher(&rotorRight, forwardPlugboard);
            TraceLog(LOG_INFO, "Middle Rotor Input: %d (%c)", forwardRight, toAscii(forwardRight));

            int forwardMiddle = forwardEncipher(&rotorMiddle, forwardRight);
            TraceLog(LOG_INFO, "Left Rotor Input: %d (%c)", forwardMiddle, toAscii(forwardMiddle));

            int forwardLeft = forwardEncipher(&rotorLeft, forwardMiddle);
            TraceLog(LOG_INFO, "Reflector Input: %d (%c)", forwardLeft, toAscii(forwardLeft));

            int reflected = forwardEncipher(&reflector, forwardLeft);
            TraceLog(LOG_INFO, "Left Rotor Input: %d (%c)", reflected, toAscii(reflected));

            int reverseLeft = reverseEncipher(&rotorLeft, reflected);
            TraceLog(LOG_INFO, "Middle Rotor Input: %d (%c)", reverseLeft, toAscii(reverseLeft));

            int reverseMiddle = reverseEncipher(&rotorMiddle, reverseLeft);
            TraceLog(LOG_INFO, "Right Rotor Input: %d (%c)", reverseMiddle, toAscii(reverseMiddle));

            int reverseRight = reverseEncipher(&rotorRight, reverseMiddle);
            TraceLog(LOG_INFO, "Plugboard Rotor Input: %d (%c)", reverseRight, toAscii(reverseRight));

            int output = forwardEncipher(&plugboard, reverseRight);
            TraceLog(LOG_INFO, "output: %d (%c)", output, toAscii(output));


        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Enigma Single Rotor", 26, 38, 29, LIGHTGRAY);

        EndDrawing();

    }

    CloseWindow();

    return 0;
}