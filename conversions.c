#include "conversions.h"

int toValue(int ascii) {
    if (ascii >= 'a' && ascii <= 'z') return (ascii - 97);
    else if (ascii >= 'A' && ascii <= 'Z') return (ascii - 65);
    return -1;
}

int toAscii(int keyValue) {
    if (keyValue >= 0 && keyValue <= 25) return (keyValue + 65);
    return -1;
}