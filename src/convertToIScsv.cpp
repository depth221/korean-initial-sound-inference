#include <iostream>
#include <fstream>
#include <vector>

int main(void) {
    std::fstream fs;
    std::string str_buf;
    fs.open("../db/most5800.csv", std::ios::in);

    std::vector<std::string> word;
    std::vector<std::string> initsound;
    while(!fs.eof()){
        getline(fs,str_buf,',');
        std::cout<<str_buf<<std::endl;        
    }
    return 0;
}