#include "conversions.h"

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