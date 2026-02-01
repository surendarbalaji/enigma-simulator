#include "conversions.h"

int toValue(int ascii) {
    if (ascii >= 'a' && ascii <= 'z') return (ascii - 96);
    else if (ascii >= 'A' && ascii <= 'Z') return (ascii - 64);
    return 0;
}

int toAscii(int keyValue) {
    if (keyValue >= 1 && keyValue <= 26) return (keyValue + 64);
    return 0;
}