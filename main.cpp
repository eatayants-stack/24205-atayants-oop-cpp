#include"Parser.h"
#include"CSV-Loader.h"
#include<iostream>
#include<fstream>
#include<string>
#include<map>

int main() {
    std::ifstream Data;
    std::string array;
    Data.open("in.txt");
    std::map<std::string, int> wordFrequencies;
    std::string line;
    while (std::getline(Data, line)) {
        parse(line, wordFrequencies);
    }
    Data.close();

    DataToCSV(wordFrequencies, "output.csv");
    return 0;
}