#pragma once

typedef struct {
    int configuration[26];
    int reverseConfiguration[26];
    int switchNotch;
    int position;
} Rotor;

Rotor InitialiseRotor(int configuration[26], int switchNotch, int position);
void turn(Rotor* rotorRight, Rotor* rotorMiddle, Rotor* rotorLeft);
int forwardEncipher(const Rotor* rotor, int input);
int reverseEncipher(const Rotor* rotor, int input);
