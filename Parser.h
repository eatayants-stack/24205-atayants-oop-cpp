#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>



class Parser {
public:

    static std::vector<std::string> parseLine(const std::string& line);
};

#endif // PARSER_H