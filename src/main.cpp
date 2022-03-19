#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>

#include "sqlite3/sqlite3.h"
#include "parseSounds.hpp"

short hangul_jamo_table[] = {1, 2, 4, 7, 8, 9, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}; // ㄱ, ㄲ, ㄴ, ㄷ, ㄸ, ㄹ, ㅁ, ㅂ, ㅃ, ㅅ, ㅆ, ㅇ, ㅈ, ㅉ, ㅊ, ㅋ, ㅌ, ㅍ, ㅎ

class initial_sound_sqlite3 {
    private:
    sqlite3* db;
    sqlite3_stmt* res;
    char* err_msg = 0;
    int rc = 0;
    std::string input_str;
    std::string input_str_initsound;
    std::vector<int> input_str_unicode;

    public:
    initial_sound_sqlite3(const char* filename);
    initial_sound_sqlite3(const char* filename, const std::string& input_str);
    int utf8ToUnicode();
    int convertInitSound();
    int open_db(const char* filename);
    int exec_sql(const char* sql, void* value);
    int compare_initsound();
    static int callback_select(void* param, int argc, char** argv, char** azColName);
};

initial_sound_sqlite3::initial_sound_sqlite3(const char* filename) {
    initial_sound_sqlite3(filename, "테스트");
}

initial_sound_sqlite3::initial_sound_sqlite3(const char* filename, const std::string& input_str) {
    this->input_str = input_str;
    this->open_db(filename);
    this->utf8ToUnicode();
    this->convertInitSound();
}

int initial_sound_sqlite3::utf8ToUnicode() {
    return parseSounds::utf8ToUnicode(this->input_str, (this->input_str).size(), this->input_str_unicode);
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

    //std::cout << input_str_initsound;
}

int initial_sound_sqlite3::open_db(const char* filename) {
    this->rc = sqlite3_open(filename, &(this->db));

    if (this->rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(this->db);
        sqlite3_close(this->db);
        
        return -1;
    }
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

int initial_sound_sqlite3::compare_initsound() {
    std::stringstream sql_stringstream;

    sql_stringstream << "SELECT * FROM noun WHERE initsound = '" << this->input_str_initsound << "';";
    this->exec_sql(sql_stringstream.str().c_str(), 0);
    return 0;
}

int initial_sound_sqlite3::callback_select(void* param, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << argv[i] << "\n";
    }
    std::cout << "\n";
    return 0;
}

int main() {
    std::string input_str;
    getline(std::cin, input_str);
    
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
        
    
    korean_db.compare_initsound();
    return 0;
}