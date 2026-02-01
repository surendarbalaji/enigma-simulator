#include "raylib.h"
#include "rotors.h"
#include "conversions.h"

#define LIGHTERGRAY (Color) { 235, 235, 235, 230 }


int main(void) {

    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Enigma Sim");

    // left-most rotor is usually rotor 3
    // Enigma I from 1930

    int rotorIII_config[26] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};
    Rotor rotorRight = InitialiseRotor(rotorIII_config,21,0);

    int rotorII_config[26] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
    Rotor rotorMiddle = InitialiseRotor(rotorII_config,4,0);

    int rotorI_config[26] = {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9};
    Rotor rotorLeft = InitialiseRotor(rotorI_config,16,0);

    // just initialising the reflector and plugboard as a (technically stationary) rotor since it doens't seem worth creating a separate struct

    int ukwb_config[26] = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19};
    Rotor reflector = InitialiseRotor(ukwb_config, -1, 0);

    Rotor plugboard_config[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    Rotor plugboard = InitialiseRotor(plugboard_config, -1, 0);


    // initialising keyboard keys

    const char* keysLine1[9] = {"Q", "W", "E", "R", "T", "Z", "U", "I", "O"};
    const char* keysLine2[8] = {"A", "S", "D", "F", "G", "H", "J", "K"};
    const char* keysLine3[9] = {"P", "X", "C", "V", "B", "N", "N", "M", "L"};

    while (!WindowShouldClose()) {

        int key = GetCharPressed();

        if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {

            // if valid input, pass through the enigma machine;

            TraceLog(LOG_INFO, "Key Pressed: %c (Ascii: %d)", key, key);

            turn(&rotorRight, &rotorMiddle, &rotorLeft);

            TraceLog(LOG_INFO, "Rotor Position %d (%c), %d (%c), %d (%c)", rotorLeft.position, toAscii(rotorLeft.position), rotorMiddle.position, toAscii(rotorMiddle.position), rotorRight.position, toAscii(rotorRight.position));

            int keyValue = toValue(key);
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

                DrawText("Enigma Keyboard", 26, 38, 29, LIGHTGRAY);

                DrawRectangle(screenWidth/4, 9*screenHeight/16, screenWidth/2, screenHeight/4, LIGHTERGRAY);

                for (int i = 1; i < 10; i++) {
                    DrawCircleLines(screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + screenHeight/16, 20.0f, LIGHTGRAY);
                    DrawText(keysLine1[i-1], screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + screenHeight/16, 15, GRAY);
                }

                for (int i = 0; i < 8; i++) {
                    DrawCircleLines( (screenWidth/4) +  (3*screenWidth/40) + (i * screenWidth/20), 9*screenHeight/16 + 2*screenHeight/16, 20.0f, LIGHTGRAY);
                    DrawText(keysLine2[i], (screenWidth/4) +  (3*screenWidth/40) + (i * screenWidth/20), 9*screenHeight/16 + 2*screenHeight/16, 15, GRAY);
                }

                for (int i = 1; i < 10; i++) {
                    DrawCircleLines(screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + 3*screenHeight/16, 20.0f, LIGHTGRAY);
                    DrawText(keysLine3[i-1], screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + 3*screenHeight/16, 15, GRAY);
                }

            EndDrawing();

        }

    CloseWindow();

    return 0;

}