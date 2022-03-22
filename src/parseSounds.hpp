#include <vector>

namespace parseSounds {
    int utf8ToUnicode(const std::string& utf8_str, int size_of_utf8, std::vector<int>& unicode_str_vec);
    int unicodeToUtf8(const std::vector<int>& unicode_str_vec, std::string& utf8_str);
    inline bool isHangul(int character) {
        return (character < 0xAC00 || character > 0xD79F) ? ((character >= 12593 && character <= 12622) ? true : false) : true;
    }
    int parseSounds(int character, int* sounds_code, int size);
}