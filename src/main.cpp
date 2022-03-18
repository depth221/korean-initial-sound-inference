#include <iostream>
#include <string>

#include "parseSounds.hpp"

int main() {
    std::string input_str;
    getline(std::cin, input_str);

    std::string input_str_init_sound(input_str);
    for (int i = 0; i < input_str.length(); i += 3) {
        int char_sounds_code[3] = {0, 0, 0};
        std::cout << utf8ToUnicode3(input_str[0], input_str[1], input_str[2]);
        if (parseSounds(utf8ToUnicode3(input_str[0], input_str[1], input_str[2]), char_sounds_code, sizeof(char_sounds_code))) {
            std::cerr << "Error: Not Hangul";
            return -1;
        } else {
            std::cout << "wow!\n";
            std::cout << char_sounds_code[0] << "\n";
            std::cout << char_sounds_code[1] << "\n";
            std::cout << char_sounds_code[2] << "\n";
        }

    }

    return 0;
}