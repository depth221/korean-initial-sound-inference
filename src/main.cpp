#include <iostream>
#include <vector>
#include <string>

#include "sqlite3/sqlite3.h"
#include "parseSounds.hpp"

class initial_sound_sqlite3 {
    private:
    sqlite3* db;
    sqlite3_stmt* res;
    char* err_msg = 0;
    int rc = 0;

    public:
    int open_db(const char* filename);
    int exec_sql(const char* sql, void* value);
    static int callback_select(void* NotUsed, int argc, char** argv, char** azColName);
};

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
}

int initial_sound_sqlite3::callback_select(void* NotUsed, int argc, char** argv, char** azColName) {
    NotUsed = 0;
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << argv[i] << "\n";
    }
    std::cout << "\n";
    return 0;
}

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
    
    // open DB
    initial_sound_sqlite3 korean_db;
    if (korean_db.open_db("../db/kr_korean.db") != 0)
        return -1;

    if (korean_db.exec_sql("PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL;", 0) != 0)
        return -1;
    if (korean_db.exec_sql("SELECT * FROM noun", 0) != 0)
        return -1;
        

    return 0;
}