#include <iostream>
#include "sqlite3/sqlite3.h"

int main(void) {
    sqlite3* db;
    sqlite3_stmt* res;
    
    int rc = sqlite3_open("../db/kr_korean.db", &db);
    
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        
        return 1;
    }
    
    rc = sqlite3_prepare_v2(db, "SELECT count(*) from kr", -1, &res, 0);    
    
    if (rc != SQLITE_OK) 
    {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        
        return 1;
    }
    
    rc = sqlite3_step(res);
    
    if (rc == SQLITE_ROW)
    {
        printf("%s\n", sqlite3_column_text(res, 0));
    }
    
    sqlite3_finalize(res);
    sqlite3_close(db);
    
    return 0;
}