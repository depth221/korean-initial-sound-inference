#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "sqlite3/sqlite3.h"

int main(void) {

    // open DB
    sqlite3* db;
    sqlite3_stmt* res;

    int rc = sqlite3_open("../db/kr_korean.db", &db);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        
        return -1;
    }

    char* err_msg = 0;
    const char* create_table_sql = "DROP TABLE IF EXISTS noun;"
                             "CREATE TABLE noun(id INT, rank INT, word TEXT, part TEXT, initsound TEXT);";

    rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table: " << sqlite3_errmsg(db);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return -1;
    }

    // open CSV
    std::ifstream fs("../db/most5800_noun.csv");
    if (fs.fail()) {
        std::cerr << "Error: File not exist";
        return -1;
    }

    std::string str_buf;

    int count = 0;
    int row = 0;
    std::vector<int> rank_csv;
    std::vector<std::string> word_csv;
    std::vector<std::string> part_csv;
    std::vector<std::string> initsound_csv;

    char* insert_sql[500];

    getline(fs, str_buf);
    while(!fs.eof()) {
        if (count == 0) {
            getline(fs, str_buf, ',');
            if (str_buf.empty()) continue;

            rank_csv.push_back(std::stoi(str_buf));
            //std::cout << "rank: " << str_buf << std::endl;
        } else if (count == 1) {
            getline(fs, str_buf, ',');
            word_csv.push_back(str_buf);
            //std::cout << "word: " << str_buf << std::endl;
        } else if (count == 2) {
            getline(fs, str_buf, ',');
            part_csv.push_back(str_buf);
            //std::cout << "part: " << str_buf << std::endl;
        } else if (count == 3) {
            getline(fs, str_buf);
            initsound_csv.push_back(str_buf);
            //std::cout << "init: " << str_buf << "\n" << std::endl;
        }

        count += 1;
        if (count == 4) {
            count = 0;
        }
    }

    for (int row = 0; row < rank_csv.size(); row++) {
        std::stringstream tmp_insert_stream;
        tmp_insert_stream << "INSERT INTO noun VALUES(";
        tmp_insert_stream << row << ", ";
        tmp_insert_stream << rank_csv[row] << ", '";
        tmp_insert_stream << word_csv[row] << "', '";
        tmp_insert_stream << part_csv[row] << "', '";
        tmp_insert_stream << initsound_csv[row] << "');";

        std::cout << tmp_insert_stream.str() << std::endl;
        rc = sqlite3_exec(db, tmp_insert_stream.str().c_str(), 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            
            return -1;
        }
    }

    fs.close();
    sqlite3_close(db);
    
    return 0;
}