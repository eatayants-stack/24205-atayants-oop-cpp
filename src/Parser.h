#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

class Parser {
public:
    vector<string> parseLine(const string& line);
};

#endif // PARSER_H