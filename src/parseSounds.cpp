#include <string>
#include <iostream>

int utf8ToUnicode3(char utf8_0, char utf8_1, char utf8_2) {
    return ((utf8_0 & 0x0F) << 12) + ((utf8_1 & 0x3F) << 6) + (utf8_2 & 0x3F);
}

int parseSounds(int character, int* sounds_code, int size) {
    if (size != 3 * sizeof(int))
        return -1;

    if (character < 0xAC00 || character > 0xD79F) { // Not Hangul
        return -1;
    } else {
        sounds_code[0] = ((character - 0xAC00) / 28) / 21;
        sounds_code[1] = ((character - 0xAC00) / 28) % 21;
        sounds_code[2] = (character - 0xAC00) % 28;
    }

    return 0;
}