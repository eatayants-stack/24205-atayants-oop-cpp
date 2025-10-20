#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>


class FileReader {
private:
    std::string filename;
    std::ifstream* file;

public:
    FileReader(const std::string& filename);
    ~FileReader();
    void open();
    void close();
    bool hasNext();
    std::string next();
    void reset();
};

#endif // FILEREADER_H
