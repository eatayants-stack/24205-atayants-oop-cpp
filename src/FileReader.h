#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <string>


class FileReader {
private:
    std::ifstream file;

public:
    explicit FileReader(const std::string& filename);
    ~FileReader();
    bool isOpen() const;
    bool isEOF();
    std::string getLine();
};

#endif // FILEREADER_H
