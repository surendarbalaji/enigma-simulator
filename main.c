#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "rotors.h"
#include "conversions.h"
#include "textWrap.h"

#define LIGHTERGRAY (Color) { 235, 235, 235, 230 }


int main(void) {

    const int screenWidth = 1600;
    const int screenHeight = 900;

    float wheelY = screenHeight / 25.0f;
    float wheelHeight = screenHeight / 2.0f;
    float narrowWidth = screenWidth / 25.0f;
    float rotorWidth = screenWidth / 12.5f;
    float letterMargin = 20.0f;
    float gap = 100.0f;

    float letterSpacing = wheelHeight / 27.0f;
    float totalWidth = narrowWidth * 2 + rotorWidth * 3 + gap * 4;

    char input[64] = "\0";
    char outputStore[64] = "\0";
    int letterCount = 0;

    int forwardPlugboard = 0;
    int forwardRight = 0;
    int forwardMiddle = 0;
    int forwardLeft = 0;
    int reflected = 0;
    int reverseLeft = 0;
    int reverseMiddle = 0;
    int reverseRight = 0;
    int output = 0;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Enigma Sim");

    Font mechanical135 = LoadFontEx("../resources/mechanical.ttf", 13.5f, 0, 0);
    Font mechanical50 = LoadFontEx("../resources/mechanical.ttf", 50.0f, 0, 0);

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

    int plugboard_config[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    Rotor plugboard = InitialiseRotor(plugboard_config, -1, 0);

    // initialising keyboard keys

    const char* keysLine1[9] = {"Q", "W", "E", "R", "T", "Z", "U", "I", "O"};
    const char* keysLine2[8] = {"A", "S", "D", "F", "G", "H", "J", "K"};
    const char* keysLine3[9] = {"P", "Y", "X", "C", "V", "B", "N", "M", "L"};

    while (!WindowShouldClose()) {

        int key = GetCharPressed();

        if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {
            // if valid input, store in input and pass through the enigma machine;

            if (letterCount == 64) {
                input[0] = '\0';
                outputStore[0] = '\0';
                letterCount = 0;
            }

            input[letterCount] = (char)key;
            input[letterCount+1] = '\0';
            letterCount++;

            TraceLog(LOG_INFO, "Key Pressed: %c (Ascii: %d)", key, key);

            turn(&rotorRight, &rotorMiddle, &rotorLeft);

            TraceLog(LOG_INFO, "Rotor Position %d (%c), %d (%c), %d (%c)", rotorLeft.position, toAscii(rotorLeft.position), rotorMiddle.position, toAscii(rotorMiddle.position), rotorRight.position, toAscii(rotorRight.position));

            int keyValue = toValue(key);
            TraceLog(LOG_INFO, "Plugboard Input: %d", keyValue);

            forwardPlugboard = forwardEncipher(&plugboard, keyValue);
            TraceLog(LOG_INFO, "Right Rotor Input: %d (%c)", forwardPlugboard, toAscii(forwardPlugboard));

            forwardRight = forwardEncipher(&rotorRight, forwardPlugboard);
            TraceLog(LOG_INFO, "Middle Rotor Input: %d (%c)", forwardRight, toAscii(forwardRight));

            forwardMiddle = forwardEncipher(&rotorMiddle, forwardRight);
            TraceLog(LOG_INFO, "Left Rotor Input: %d (%c)", forwardMiddle, toAscii(forwardMiddle));

            forwardLeft = forwardEncipher(&rotorLeft, forwardMiddle);
            TraceLog(LOG_INFO, "Reflector Input: %d (%c)", forwardLeft, toAscii(forwardLeft));

            reflected = forwardEncipher(&reflector, forwardLeft);
            TraceLog(LOG_INFO, "Left Rotor Input: %d (%c)", reflected, toAscii(reflected));

            reverseLeft = reverseEncipher(&rotorLeft, reflected);
            TraceLog(LOG_INFO, "Middle Rotor Input: %d (%c)", reverseLeft, toAscii(reverseLeft));

            reverseMiddle = reverseEncipher(&rotorMiddle, reverseLeft);
            TraceLog(LOG_INFO, "Right Rotor Input: %d (%c)", reverseMiddle, toAscii(reverseMiddle));

            reverseRight = reverseEncipher(&rotorRight, reverseMiddle);
            TraceLog(LOG_INFO, "Plugboard Rotor Input: %d (%c)", reverseRight, toAscii(reverseRight));

            output = forwardEncipher(&plugboard, reverseRight);
            TraceLog(LOG_INFO, "output: %d (%c)", output, toAscii(output));

            outputStore[letterCount - 1] = toAscii(output);
            outputStore[letterCount] = '\0';

        }

        if (IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            input[letterCount] = '\0';
        }

            BeginDrawing();

                ClearBackground(RAYWHITE);

                DrawText("Enigma I", 26, 38, 29, LIGHTGRAY);

                // reflector wheel
                Rectangle reflectorWheel = (Rectangle){(screenWidth - totalWidth) / 2.0f, wheelY, narrowWidth, wheelHeight};
                DrawRectangleRec(reflectorWheel, LIGHTGRAY);
                DrawRectangleLinesEx(reflectorWheel, 3, GRAY);

                for (int i = 0; i < 26; i++) {
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + i), (Vector2){reflectorWheel.x - 2.0f + reflectorWheel.width / 2.0f,wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                }

                // left rotor wheel
                Rectangle leftRotorWheel = (Rectangle){reflectorWheel.x + narrowWidth + gap, wheelY, rotorWidth, wheelHeight};
                DrawRectangleRec(leftRotorWheel, LIGHTGRAY);
                DrawRectangleLinesEx(leftRotorWheel, 3, GRAY);

                DrawRectangleLinesEx((Rectangle){leftRotorWheel.x - 5.0f, wheelY + wheelHeight - 22.0f, rotorWidth + 10.0f, 17.0f}, 2.5f, GRAY);

                for (int i = 0; i < 26; i++) {
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + rotorLeft.configuration[(i + rotorLeft.position) % 26]), (Vector2){leftRotorWheel.x + 22, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + ((i + rotorLeft.position) % 26)), (Vector2){leftRotorWheel.x + leftRotorWheel.width - 32, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                }

                // middle rotor wheel
                Rectangle middleRotorWheel = (Rectangle){leftRotorWheel.x + rotorWidth + gap, wheelY, rotorWidth, wheelHeight};
                DrawRectangleRec(middleRotorWheel, LIGHTGRAY);
                DrawRectangleLinesEx(middleRotorWheel, 3, GRAY);

                DrawRectangleLinesEx((Rectangle){middleRotorWheel.x - 5.0f, wheelY + wheelHeight - 22.0f, rotorWidth + 10.0f, 17.0f}, 2.5f, GRAY);

                for (int i = 0; i < 26; i++) {
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + rotorMiddle.configuration[(i + rotorMiddle.position) % 26]), (Vector2){middleRotorWheel.x + 22, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + ((i + rotorMiddle.position) % 26)), (Vector2){middleRotorWheel.x + middleRotorWheel.width - 32, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                }

                // right rotor wheel
                Rectangle rightRotorWheel = (Rectangle){middleRotorWheel.x + rotorWidth + gap, wheelY, rotorWidth, wheelHeight};
                DrawRectangleRec(rightRotorWheel, LIGHTGRAY);
                DrawRectangleLinesEx(rightRotorWheel, 3, GRAY);

                // current rotor setting
                DrawRectangleLinesEx((Rectangle){rightRotorWheel.x - 5.0f, wheelY + wheelHeight - 22.0f, rotorWidth + 10.0f, 17.0f}, 2.5f, GRAY);

                for (int i = 0; i < 26; i++) {
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + rotorRight.configuration[(i + rotorRight.position) % 26]), (Vector2){rightRotorWheel.x + 22, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + ((i + rotorRight.position) % 26)), (Vector2){rightRotorWheel.x + rightRotorWheel.width - 32, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 0.5f, GRAY);
                }

                // static wheel
                Rectangle staticWheel = (Rectangle){rightRotorWheel.x + rotorWidth + gap, wheelY, narrowWidth, wheelHeight};
                DrawRectangleRec(staticWheel, LIGHTGRAY);
                DrawRectangleLinesEx(staticWheel, 3, GRAY);

                for (int i = 0; i < 26; i++) {
                    DrawTextEx(mechanical135, TextFormat("%c", 'A' + i), (Vector2){staticWheel.x - 2.0f + staticWheel.width / 2.0f, wheelY + wheelHeight - letterMargin - i * letterSpacing}, 13.5f, 1.0f, GRAY);
                }

                // keyboard
                Rectangle keyboard = (Rectangle){screenWidth/4, 9*screenHeight/16, screenWidth/2, screenHeight/4};
                DrawRectangleRec(keyboard, LIGHTERGRAY);
                DrawRectangleLinesEx(keyboard, 2, LIGHTGRAY);

                for (int i = 1; i < 10; i++) {

                    if (keysLine1[i-1][0] == toUpper(input[letterCount - 1])) DrawCircleLines(screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + screenHeight/16, 20.0f, GRAY);
                    else DrawCircleLines(screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + screenHeight/16, 20.0f, LIGHTGRAY);

                    DrawTextEx(mechanical135, keysLine1[i-1], (Vector2){screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + screenHeight/16}, 13.5, 1.0f, GRAY);
                }


                for (int i = 0; i < 8; i++) {

                    if (keysLine2[i-1][0] == toUpper(input[letterCount - 1])) DrawCircleLines( (screenWidth/4) +  (3*screenWidth/40) + (i * screenWidth/20), 9*screenHeight/16 + 2*screenHeight/16, 20.0f, GRAY);
                    else DrawCircleLines( (screenWidth/4) +  (3*screenWidth/40) + (i * screenWidth/20), 9*screenHeight/16 + 2*screenHeight/16, 20.0f, LIGHTGRAY);

                    DrawTextEx(mechanical135, keysLine2[i], (Vector2){(screenWidth/4) +  (3*screenWidth/40) + (i * screenWidth/20), 9*screenHeight/16 + 2*screenHeight/16}, 13.5, 1.0f, GRAY);
                }

                for (int i = 1; i < 10; i++) {

                    if (keysLine3[i-1][0] == toUpper(input[letterCount-1])) DrawCircleLines(screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + 3*screenHeight/16, 20.0f, GRAY);
                    else DrawCircleLines(screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + 3*screenHeight/16, 20.0f, LIGHTGRAY);

                    DrawTextEx(mechanical135, keysLine3[i-1], (Vector2){screenWidth/4 + i * screenWidth/20, 9*screenHeight/16 + 3*screenHeight/16}, 13.5, 1.0f, GRAY);
                }

                // plugboard
                Rectangle plugboardRectangle = (Rectangle){(screenWidth - plugboardRectangle.width) / 2.0f, keyboard.y + keyboard.height + 20.0f, 2 * screenWidth / 5, screenHeight / 7};
                DrawRectangleRec(plugboardRectangle, LIGHTERGRAY);
                DrawRectangleLinesEx(plugboardRectangle, 2, LIGHTGRAY);

                float plugboardKeysX = plugboardRectangle.x + 60.0f;
                float plugboardKeySpacing = plugboardRectangle.width / 10.0f;

                for (int i = 0; i < 9; i++) {
                    DrawTextEx(mechanical135, keysLine1[i], (Vector2){plugboardKeysX + i * plugboardKeySpacing, plugboardRectangle.y + 20.0f}, 13.5f, 1.0f, GRAY);
                    DrawCircle(plugboardKeysX + i * plugboardKeySpacing, plugboardRectangle.y + 38.0f, 4.0f, Fade(LIGHTGRAY, 0.6f));
                    DrawCircle(plugboardKeysX + i * plugboardKeySpacing, plugboardRectangle.y + 38.0f, 2.0f, Fade(GRAY, 0.2f));
                }

                for (int i = 0; i < 8; i++) {
                    DrawTextEx(mechanical135, keysLine2[i], (Vector2){plugboardKeysX + 30.0f + i * plugboardKeySpacing, plugboardRectangle.y + 55.0f}, 13.5f, 1.0f, GRAY);
                    DrawCircle(plugboardKeysX + 30.0f + i * plugboardKeySpacing, plugboardRectangle.y + 73.0f, 4.0f, Fade(LIGHTGRAY, 0.6f));
                    DrawCircle(plugboardKeysX + 30.0f + i * plugboardKeySpacing, plugboardRectangle.y + 73.0f, 2.0f, Fade(GRAY, 0.2f));
                }

                for (int i = 0; i < 9; i++) {
                    DrawTextEx(mechanical135, keysLine3[i], (Vector2){plugboardKeysX + i * plugboardKeySpacing, plugboardRectangle.y + 90.0f}, 13.5f, 1.0f, GRAY);
                    DrawCircle(plugboardKeysX + i * plugboardKeySpacing, plugboardRectangle.y + 108.0f, 4.0f, Fade(LIGHTGRAY, 0.6f));
                    DrawCircle(plugboardKeysX + i * plugboardKeySpacing, plugboardRectangle.y + 108.0f, 2.0f, Fade(GRAY, 0.2f));
                }

                // DrawTextEx(mechanical50, TextFormat("%s", outputStore), (Vector2){staticWheel.x + 100.0f, staticWheel.y + 20.0f}, 50.0f, 5.0f, LIGHTGRAY);
                Rectangle outputBox = (Rectangle){staticWheel.x + 100.0f, staticWheel.y + 20.0f, 280.0f, staticWheel.height};
                DrawTextBoxed(mechanical50, outputStore, outputBox, 50.0f, 5.0f, false, LIGHTGRAY);

                if (letterCount > 0) {

                    // this is an abomination, but it's my abomination AND IT WORKS

                    float wireThickness = 4.0f;

                    // keyboard to plugboard.
                    DrawLineEx((Vector2){screenWidth/2, keyboard.y  + keyboard.height}, (Vector2){screenWidth/2.0f, plugboardRectangle.y}, wireThickness, GRAY);

                    // plugboard to static wheel
                    DrawLineEx((Vector2){plugboardRectangle.x + plugboardRectangle.width, plugboardRectangle.y + plugboardRectangle.height/2.0f}, (Vector2){9*screenWidth/10, plugboardRectangle.y + plugboardRectangle.height/2.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){9*screenWidth/10, plugboardRectangle.y + plugboardRectangle.height/2.0f}, (Vector2){9*screenWidth/10, wheelY + wheelHeight - letterMargin - (toValue(input[letterCount-1]) * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){9*screenWidth/10, wheelY + wheelHeight - letterMargin - (toValue(input[letterCount-1]) * letterSpacing) + 5.0f}, (Vector2){staticWheel.x + staticWheel.width, wheelY + wheelHeight - letterMargin - (toValue(input[letterCount-1]) * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // static wheel to right rotor
                    DrawLineEx((Vector2){staticWheel.x, wheelY + wheelHeight - letterMargin - (forwardPlugboard * letterSpacing) + 5.0f}, (Vector2){rightRotorWheel.x + rightRotorWheel.width, wheelY + wheelHeight - letterMargin - (forwardPlugboard * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // right rotor to middle rotor (The Gap Stair-Step)
                    DrawLineEx((Vector2){rightRotorWheel.x, wheelY + wheelHeight - letterMargin - (forwardPlugboard * letterSpacing) + 5.0f}, (Vector2){rightRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardPlugboard * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){rightRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardPlugboard * letterSpacing) + 5.0f}, (Vector2){rightRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardRight * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){rightRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardRight * letterSpacing) + 5.0f}, (Vector2){middleRotorWheel.x + middleRotorWheel.width, wheelY + wheelHeight - letterMargin - (forwardRight * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // middle rotor to left rotor (The Gap Stair-Step)
                    DrawLineEx((Vector2){middleRotorWheel.x, wheelY + wheelHeight - letterMargin - (forwardRight * letterSpacing) + 5.0f}, (Vector2){middleRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardRight * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){middleRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardRight * letterSpacing) + 5.0f}, (Vector2){middleRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardMiddle * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){middleRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardMiddle * letterSpacing) + 5.0f}, (Vector2){leftRotorWheel.x + leftRotorWheel.width, wheelY + wheelHeight - letterMargin - (forwardMiddle * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // left rotor to reflector (The Gap Stair-Step)
                    DrawLineEx((Vector2){leftRotorWheel.x, wheelY + wheelHeight - letterMargin - (forwardMiddle * letterSpacing) + 5.0f}, (Vector2){leftRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardMiddle * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){leftRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardMiddle * letterSpacing) + 5.0f}, (Vector2){leftRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardLeft * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){leftRotorWheel.x - 20.0f, wheelY + wheelHeight - letterMargin - (forwardLeft * letterSpacing) + 5.0f}, (Vector2){reflectorWheel.x + reflectorWheel.width, wheelY + wheelHeight - letterMargin - (forwardLeft * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // reflector to reflector
                    DrawLineEx((Vector2){reflectorWheel.x, wheelY + wheelHeight - letterMargin - (forwardLeft * letterSpacing) + 5.0f}, (Vector2){reflectorWheel.x - 40.0f, wheelY + wheelHeight - letterMargin - (forwardLeft * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){reflectorWheel.x - 40.0f, wheelY + wheelHeight - letterMargin - (forwardLeft * letterSpacing) + 5.0f}, (Vector2){reflectorWheel.x - 40.0f, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){reflectorWheel.x - 40.0f, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, (Vector2){reflectorWheel.x, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // reflector to left rotor (Return Gap - Straight across because drop happens AFTER the rotor)
                    DrawLineEx((Vector2){reflectorWheel.x + reflectorWheel.width, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, (Vector2){leftRotorWheel.x, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // left rotor to middle rotor (Return Gap Stair-Step)
                    DrawLineEx((Vector2){leftRotorWheel.x + leftRotorWheel.width, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, (Vector2){leftRotorWheel.x + leftRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){leftRotorWheel.x + leftRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reflected * letterSpacing) + 5.0f}, (Vector2){leftRotorWheel.x + leftRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseLeft * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){leftRotorWheel.x + leftRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseLeft * letterSpacing) + 5.0f}, (Vector2){middleRotorWheel.x, wheelY + wheelHeight - letterMargin - (reverseLeft * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // middle rotor to right rotor (Return Gap Stair-Step)
                    DrawLineEx((Vector2){middleRotorWheel.x + middleRotorWheel.width, wheelY + wheelHeight - letterMargin - (reverseLeft * letterSpacing) + 5.0f}, (Vector2){middleRotorWheel.x + middleRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseLeft * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){middleRotorWheel.x + middleRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseLeft * letterSpacing) + 5.0f}, (Vector2){middleRotorWheel.x + middleRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseMiddle * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){middleRotorWheel.x + middleRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseMiddle * letterSpacing) + 5.0f}, (Vector2){rightRotorWheel.x, wheelY + wheelHeight - letterMargin - (reverseMiddle * letterSpacing) + 5.0f}, wireThickness, GRAY);

                    // right rotor to static wheel (Return Gap Stair-Step)
                    DrawLineEx((Vector2){rightRotorWheel.x + rightRotorWheel.width, wheelY + wheelHeight - letterMargin - (reverseMiddle * letterSpacing) + 5.0f}, (Vector2){rightRotorWheel.x + rightRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseMiddle * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){rightRotorWheel.x + rightRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseMiddle * letterSpacing) + 5.0f}, (Vector2){rightRotorWheel.x + rightRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseRight * letterSpacing) + 5.0f}, wireThickness, GRAY);
                    DrawLineEx((Vector2){rightRotorWheel.x + rightRotorWheel.width + 20.0f, wheelY + wheelHeight - letterMargin - (reverseRight * letterSpacing) + 5.0f}, (Vector2){staticWheel.x, wheelY + wheelHeight - letterMargin - (reverseRight * letterSpacing) + 5.0f}, wireThickness, GRAY);
                }

            EndDrawing();

        }

    UnloadFont(mechanical135);
    CloseWindow();

    return 0;

}