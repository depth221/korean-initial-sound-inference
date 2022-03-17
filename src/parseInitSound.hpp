#include <string>

int parseInitSound(wchar_t character, wchar_t* init_sound) {
    if (character < 0xAC00) { // Not Hangul
        init_sound = L"";
        return -1;
    } else if (character < 0xB098) { // ㄱ ㄲ?
        init_sound = L"ㄱ";
    } else if (character < 0xB098) { // ㄴ
        init_sound = L"ㄴ";
    } else if (character < 0xB2E4) { // ㄷ ㄸ?
        init_sound = L"ㄷ";
    } else if (character < 0xB77C) { // ㄹ
        init_sound = L"ㄹ";
    } else if (character < 0xB9C8) { // ㅁ
        init_sound = L"ㅁ";
    } else if (character < 0xBC14) { // ㅂ ㅃ?
        init_sound = L"ㅂ";
    } else if (character < 0xB77C) { // ㅅ ㅆ?
        init_sound = L"ㅅ";
    } else if (character < 0xB77C) { // ㅇ
        init_sound = L"ㅇ";
    } else if (character < 0xB77C) { // ㅈ ㅉ?
        init_sound = L"ㅈ";
    }

    return 0;
}