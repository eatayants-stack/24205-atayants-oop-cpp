#include "FileReader.h"
#include <string>

FileReader::FileReader(const std::string& filename)
    : file(filename) {
}

bool FileReader::isOpen() const {
    return file.is_open();
}

bool FileReader::isEOF() {
    return !file || file.peek() == EOF;
}

std::string FileReader::getLine() {
    std::string line;
    std::getline(file, line);
    return line;
}
