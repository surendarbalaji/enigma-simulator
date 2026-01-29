#include <stdio.h>
#include "raylib.h"


typedef struct {
    int configuration[26];
    int switchNotch;
    int turnSetting;
} Rotor;

int rotorPass(Rotor* rotor, int input) {

    rotor->turnSetting++;
    return rotor->configuration[input + rotor->turnSetting - 1];

}

int toValue(int ascii) {

    int keyValue = 0;

    if (ascii >= 'a' && ascii <= 'z') keyValue = ascii - 96;
    else if (ascii >= 'A' && ascii <= 'Z') keyValue = ascii - 64;

    return keyValue;

}

int toAscii(int keyValue) {

    int character = 0;

    if (keyValue >= 1 && keyValue <= 26) character = keyValue + 64;

    return character;

}


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

    Rotor rotor1;
    int rotor1_setting[26] = {4, 13, 20, 23, 19, 9, 12, 18, 21, 25, 17, 14, 11, 6, 5, 10, 3, 1, 26, 2, 16, 7, 24, 15, 8, 22};
    rotor1.switchNotch = 18;
    rotor1.turnSetting = 0;

    for (int i = 0; i < 26; i++ ) {
        rotor1.configuration[i] = rotor1_setting[i];
    }

    while (!WindowShouldClose()) {

        if (IsAnyKeyPressed()) {
            int keyValue = 0;
            int key = GetCharPressed();
            if (key > 0) TraceLog(LOG_INFO, "Key Pressed: %c (Ascii: %d)", key, key);

            keyValue = toValue(key);

            if (key > 0) TraceLog(LOG_INFO, "Key Value: %d", keyValue);

            int ciphered = rotorPass(&rotor1, keyValue);

            if (ciphered > 0) TraceLog(LOG_INFO, "Ciphered Value: %d", ciphered);
            if (ciphered > 0) TraceLog(LOG_INFO, "Ciphered Character: %c (%d)", toAscii(ciphered), toAscii(ciphered));

        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Enigma Single Rotor", 26, 38, 29, LIGHTGRAY);

        EndDrawing();

    }

    CloseWindow();

    return 0;
}
