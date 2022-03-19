#include <vector>

namespace parseSounds {
    int utf8ToUnicode(const std::string& utf8_str, int size_of_utf8, std::vector<int>& unicode_str_vec);
    int unicodeToUtf8(const std::vector<int>& unicode_str_vec, std::string& utf8_str);
    int parseSounds(int character, int* sounds_code, int size);
}