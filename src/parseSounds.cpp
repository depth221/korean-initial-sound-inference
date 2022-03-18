#include <string>
#include <list>
#include <vector>
#include <iostream>

int utf8ToUnicode(std::string utf8_str, int size_of_utf8, std::vector<int>& unicode_str_vec) {
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