#include <string>
#include <list>
#include <vector>
#include <iostream>

int utf8ToUnicode(const std::string& utf8_str, int size_of_utf8, std::vector<int>& unicode_str_vec) {
    std::list<int> unicode_str;

    for (int i = 0; i < size_of_utf8;) {
        if ((utf8_str[i] & 0x80) == 0x00) { // ~ 7 bits
            i += 1;
        } else {
            if (i == size_of_utf8 - 1) {
                std::cerr << "Error: input is not utf-8 (8-11 bits)";
                return -1;
            }

            if ((utf8_str[i] & 0x20) == 0x00) { // 8 ~ 11 bits
                unicode_str.push_back(((utf8_str[i] & 0x1F) << 6) + (utf8_str[i + 1] & 0x3F));
                i += 2;
            } else {
                if (i == size_of_utf8 - 2) {
                    std::cerr << "Error: input is not utf-8 (12-16 bits)";
                    return -1;
                }

                if ((utf8_str[i] & 0x10) == 0x00) { // 12 ~ 16 bits
                    unicode_str.push_back(((utf8_str[i] & 0x0F) << 12) + ((utf8_str[i + 1] & 0x3F) << 6) + (utf8_str[i + 2] & 0x3F));
                    i += 3;
                } else {
                    if (i == size_of_utf8 - 3) {
                        std::cerr << "Error: input is not utf-8 (17-21 bits)";
                        return -1;
                    }

                    if ((utf8_str[i] & 0x08) == 0x00) { // 17 ~ 21 bits
                        unicode_str.push_back(((utf8_str[i] & 0x0F) << 18) + ((utf8_str[i + 1] & 0x3F) << 12) + ((utf8_str[i + 2] & 0x3F) << 6) + (utf8_str[i + 3] & 0x3F));
                        i += 4;
                    } else {
                        std::cerr << "Error: input is not utf-8 (over 21 bits)";
                        return -1;
                    }
                }
            }
        }
    }
    unicode_str_vec.resize(unicode_str.size());
    std::copy(unicode_str.begin(), unicode_str.end(), unicode_str_vec.begin());

    return 0;
}

int unicodeToUtf8(const std::vector<int>& unicode_str_vec, std::string& utf8_str) {
    for (int num : unicode_str_vec) {
        char tmp_char[5] = {0, };
        std::cerr << num << std::endl;
        if (num < 1) {
            std::cerr << "Error: input is under 1";
            return -1;
        }

        if (num < (1 << 7)) {
            tmp_char[0] = num;
            tmp_char[1] = 0;
        } else if (num < (1 << 11)) {
            tmp_char[0] = 0xC0 + ((num & 0x7C0) >> 6);
            tmp_char[1] = 0xC0 + (num & 0x3F);
            tmp_char[2] = 0;
        } else if (num < (1 << 16)) {
            tmp_char[0] = 0xE0 + ((num & 0xF000) >> 12);
            tmp_char[1] = 0x80 + ((num & 0xFC0) >> 6);
            tmp_char[2] = 0x80 + (num & 0x3F); //1100 0000 + 0011 1111
            tmp_char[3] = 0;
        } else if (num < (1 << 21)) { //0 00/00 0000 /0000 00/00 0000
            tmp_char[0] = 0xF0 + ((num & 0x1C0000) >> 18);
            tmp_char[1] = 0x80 + ((num & 0x3F000) >> 12);
            tmp_char[2] = 0x80 + ((num & 0xFC0) >> 6);
            tmp_char[3] = 0x80 + (num & 0x3F);
            tmp_char[4] = 0;
        } else {
            std::cerr << "Error: input is over 21 bits";
            return -1;
        }

        utf8_str.clear();
        utf8_str.append(tmp_char);
    }
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