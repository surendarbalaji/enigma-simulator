#include "rotors.h"

Rotor InitialiseRotor(int configuration[26], int switchNotch, int position) {
    Rotor rotor;

    for (int i = 0; i < 26; i++ ) {
        rotor.configuration[i] = configuration[i];
    }

    rotor.switchNotch = switchNotch; // the location of the physical notch which allows for the subsequent rotor to rotate
    rotor.position = position; // current position of the rotor

    return rotor; // i used this method of returning fake objects in the constructions code which seemed to work well since these structs are fairly small
}

void turn(Rotor* rotorRight, Rotor* rotorMiddle, Rotor* rotorLeft) {
    rotorRight->position = (rotorRight->position + 1) % 26;

    if (rotorRight->position == rotorRight->switchNotch) {
        rotorMiddle->position = (rotorMiddle->position + 1) % 26;

        if (rotorMiddle->position == rotorMiddle->switchNotch) {
            rotorLeft->position = (rotorLeft->position + 1) % 26;
        }
    }
}

int forwardEncipher(const Rotor* rotor, int input) {
    return (rotor->configuration[input + rotor->position - 1] - rotor->position);
}
