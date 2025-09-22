#include"Parser.h"
#include"CSV-Loader.h"
#include"TotalWords.h"
#include"MergeWordCount.h"
#include<iostream>
#include<fstream>
#include<string>
#include<map>

int main() {
    std::ifstream file;
    std::string array;
    file.open("C:/Users/user/CLionProjects/24205-atayants-oop-cpp/in.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Couldn't open file!" << std::endl;
        return 1;
    }
    std::map<std::string, int> wordFrequencies;
    std::string line;
    while (std::getline(file, line)) {
        std::map<std::string, int> lineFrequencies = Parse(line);
        MergeWordCounts(wordFrequencies, lineFrequencies);
    }
    file.close();
    int WordsCount = TotalWords(wordFrequencies);
    if (WordsCount > 0) {
        DataToCSV(wordFrequencies, WordsCount, "output.csv");
    } else {
        std::cout << "File is empty." << std::endl;
    }
    return 0;
}
