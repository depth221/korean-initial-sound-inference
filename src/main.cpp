#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
#include <list>
#include <string>
#include <sstream>

#include "sqlite3/sqlite3.h"
#include "parseSounds.hpp"

short hangul_jamo_table[] = {1, 2, 4, 7, 8, 9, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}; // ㄱ, ㄲ, ㄴ, ㄷ, ㄸ, ㄹ, ㅁ, ㅂ, ㅃ, ㅅ, ㅆ, ㅇ, ㅈ, ㅉ, ㅊ, ㅋ, ㅌ, ㅍ, ㅎ
char punctuation_mark[] = {'.', ',', '?', '!', '~', ':'};

class initial_sound_sqlite3 {
    private:
    sqlite3* db;
    sqlite3_stmt* res;
    char* err_msg = 0;
    int rc = 0;
    std::string input_str_initsound;
    std::vector<int> input_str_unicode;

    int word_count = 0;
    std::vector<std::vector<int>> input_str_per_word;
    std::vector<bool> input_str_is_hangul;
    std::vector<bool> input_str_is_before_space;

    public:
    initial_sound_sqlite3(const char* filename);
    initial_sound_sqlite3(const char* filename, const std::string& input_str);
    int utf8ToUnicode();
    int split_per_word();
    int convertInitSound();
    int open_db(const char* filename);
    int exec_sql(const char* sql, void* value);
    int find_initsound();
    static int callback_select(void* param, int argc, char** argv, char** azColName);
};

initial_sound_sqlite3::initial_sound_sqlite3(const char* filename) {
    initial_sound_sqlite3(filename, "테스트");
}

initial_sound_sqlite3::initial_sound_sqlite3(const char* filename, const std::string& input_str_initsound) {
    //this->input_str = input_str;
    this->input_str_initsound = input_str_initsound;
    this->open_db(filename);
    this->utf8ToUnicode();
    this->split_per_word();
}

int initial_sound_sqlite3::utf8ToUnicode() {
    return parseSounds::utf8ToUnicode(this->input_str_initsound, (this->input_str_initsound).size(), this->input_str_unicode);
}

int initial_sound_sqlite3::split_per_word() {
    std::list<int> tmp_input_str_unicode_per_word;
    std::vector<int> tmp_input_str_unicode_per_word_vec;
    std::list<bool> tmp_input_str_unicode_is_hangul;
    std::list<bool> tmp_input_str_unicode_is_punc;
    std::string tmp_str = "";
    int lang_mode = 0; // 0: Hangul, 1: Latin Script or etc., 2: space, 3: punctuation mark

    for (int char_code : input_str_unicode) {
        if (parseSounds::isHangul(char_code)) { // Hangul (0)
            if (lang_mode == 0) {
                
            } else if (lang_mode == 1) {
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(false);
                tmp_input_str_unicode_is_punc.push_back(false);
            } else if (lang_mode == 2) {

            } else { // lang_mode == 3
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(false);
                tmp_input_str_unicode_is_punc.push_back(true);
            }
            tmp_input_str_unicode_per_word.push_back(char_code);
            lang_mode = 0;
        } else if (char_code == static_cast<int>(' ')) { // space
            if (lang_mode == 2) { // succesive spaces
                continue;
            } else if (lang_mode == 0) {
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(true);
                tmp_input_str_unicode_is_punc.push_back(false);
            } else if (lang_mode == 1) {
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(false);
                tmp_input_str_unicode_is_punc.push_back(false);
            } else { // lang_mode == 3
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(false);
                tmp_input_str_unicode_is_punc.push_back(true);
            }
            lang_mode = 2;
        } else if (std::find(punctuation_mark, punctuation_mark + (sizeof(punctuation_mark) / sizeof(char)), char_code) != \
                                               punctuation_mark + (sizeof(punctuation_mark) / sizeof(char))) { // is a punctuation mark
            if (lang_mode == 0) {
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(true);
                tmp_input_str_unicode_is_punc.push_back(false);
            } else if (lang_mode == 1) {
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(false);
                tmp_input_str_unicode_is_punc.push_back(false);
            } else if (lang_mode == 2) {

            } else { // lang_mode == 3

            }
            tmp_input_str_unicode_per_word.push_back(char_code);
            lang_mode = 3;
        } else { // Latin Script or etc.
            if (lang_mode == 0) {
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(true);
                tmp_input_str_unicode_is_punc.push_back(false);
            } else if (lang_mode == 1) {

            } else if (lang_mode == 2) {

            } else { // lang_mode == 3
                this->word_count += 1;
                tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
                std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
                input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);
                tmp_input_str_unicode_per_word.clear();
                tmp_input_str_unicode_per_word_vec.clear();

                tmp_input_str_unicode_is_hangul.push_back(false);
                tmp_input_str_unicode_is_punc.push_back(true);
            }
            tmp_input_str_unicode_per_word.push_back(char_code);
            lang_mode = 1;
        }
    }

    if (lang_mode == 0) {
        this->word_count += 1;
        tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
        std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
        input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);

        tmp_input_str_unicode_is_hangul.push_back(true);
        tmp_input_str_unicode_is_punc.push_back(false);
    } else if (lang_mode == 1) {
        this->word_count += 1;
        tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
        std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
        input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);

        tmp_input_str_unicode_is_hangul.push_back(false);
        tmp_input_str_unicode_is_punc.push_back(false);
    } else if (lang_mode == 2) {

    } else { // lang_mode == 3
        this->word_count += 1;
        tmp_input_str_unicode_per_word_vec.resize(tmp_input_str_unicode_per_word.size());
        std::copy(tmp_input_str_unicode_per_word.begin(), tmp_input_str_unicode_per_word.end(), tmp_input_str_unicode_per_word_vec.begin());
        input_str_per_word.push_back(tmp_input_str_unicode_per_word_vec);

        tmp_input_str_unicode_is_hangul.push_back(false);
        tmp_input_str_unicode_is_punc.push_back(true);
    }

    std::cout << this->word_count;

    for (auto i : input_str_per_word) {
        std::string tmp_str;
        for (auto j : i) {
            std::cout << j << " ";
        }
        parseSounds::unicodeToUtf8(i, tmp_str);
        std::cout << tmp_str;
        std::cout << "\n";
    }

    return 0;
}

int initial_sound_sqlite3::convertInitSound() {
    for (int i = 0; i < input_str_unicode.size(); i++) {
        int char_sounds_code[3] = {0, 0, 0};
        
        if (parseSounds::parseSounds(input_str_unicode[i], char_sounds_code, sizeof(char_sounds_code))) { // if not Hangul
            continue;
        } else {
            //std::cout << static_cast<int>(char_sounds_code[0]) << "\n";
            //std::cout << static_cast<int>(char_sounds_code[1]) << "\n";
            //std::cout << static_cast<int>(char_sounds_code[2]) << "\n" << std::endl;
        }

        std::vector<int> tmp_input_one_int;
        std::string tmp_input_one_str;
        tmp_input_one_int.push_back(0x3130 + hangul_jamo_table[char_sounds_code[0]]);
        parseSounds::unicodeToUtf8(tmp_input_one_int, tmp_input_one_str);
        this->input_str_initsound.append(tmp_input_one_str);
        std::cerr << tmp_input_one_str;
    }

    return 0;
}

int initial_sound_sqlite3::open_db(const char* filename) {
    this->rc = sqlite3_open(filename, &(this->db));

    if (this->rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(this->db);
        sqlite3_close(this->db);
        
        return -1;
    }

    return 0;
}

int initial_sound_sqlite3::exec_sql(const char* sql, void* value) {
    this->rc = sqlite3_exec(this->db, sql, initial_sound_sqlite3::callback_select, value, &(this->err_msg));
    if (this->rc != SQLITE_OK)
    {
        std::cerr << "Cannot execute SQL query: " << sqlite3_errmsg(this->db);
        sqlite3_close(this->db);
        
        return -1;
    }

    return 0;
}

int initial_sound_sqlite3::find_initsound() {
    std::stringstream sql_stringstream;

    sql_stringstream << "SELECT DISTINCT word FROM noun WHERE initsound = '" << this->input_str_initsound << "' ORDER BY rank;";
    this->exec_sql(sql_stringstream.str().c_str(), 0);
    return 0;
}

int initial_sound_sqlite3::callback_select(void* param, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        //if (i == 2)
            //std::cout << azColName[i] << ": " << argv[i] << "\n";
            std::cout << argv[i];
    }
    std::cout << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    std::string input_str;
    if (argc == 2) {
        input_str += argv[1];
    } else {
        getline(std::cin, input_str);
    }
    
    // open DB
    initial_sound_sqlite3 korean_db("../db/kr_korean.db", input_str);

    if (korean_db.exec_sql("PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL;", 0) != 0) {
        std::cerr << "Error: fail to execute: \nPRAGMA journal_mode=WAL; \nPRAGMA synchronous=NORMAL";
        return -1;
    }
    //if (korean_db.exec_sql("SELECT * FROM noun;", 0) != 0) {
    //    std::cerr << "Error: fail to execute: SELECT * FROM noun;";
    //    return -1;
    //}
        
    
    korean_db.find_initsound();
    return 0;
}