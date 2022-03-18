#include <iostream>
#include <vector>
#include <string>

#include "parseSounds.hpp"

int main() {
    std::string input_str;
    std::vector<int> input_str_unicode;
    getline(std::cin, input_str);
    utf8ToUnicode(input_str, input_str.size(), input_str_unicode);

    std::vector<int> input_str_init_sound(input_str_unicode);
    for (int i = 0; i < input_str_unicode.size(); i++) {
        int char_sounds_code[3] = {0, 0, 0};
        
        if (parseSounds(input_str_unicode[i], char_sounds_code, sizeof(char_sounds_code))) { // if not Hangul
            continue;
        } else {
            std::cout << static_cast<int>(char_sounds_code[0]) << "\n";
            std::cout << static_cast<int>(char_sounds_code[1]) << "\n";
            std::cout << static_cast<int>(char_sounds_code[2]) << "\n" << std::endl;

            
        }

    }

    return 0;
}