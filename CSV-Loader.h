#ifndef INC_24205_ATAYANTS_OOP_CPP_CSV_LOADER_H
#define INC_24205_ATAYANTS_OOP_CPP_CSV_LOADER_H
#include<fstream>
#include<iostream>
#include<map>
void DataToCSV(const std::map<std::string, int>& wordFrequencies, int TotalWords, const std::string& filename);
#endif //INC_24205_ATAYANTS_OOP_CPP_CSV_LOADER_H