#include "FileReader.h"
#include <iostream>

using namespace std;

FileReader::FileReader(const string& filename) : filename(filename), file(nullptr) {}

FileReader::~FileReader() {
    close();
}

void FileReader::open() {
    if (file != nullptr) {
        close();
    }
    file = new ifstream(filename);
    if (!file->is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        delete file;
        file = nullptr;
        exit(1);
    }
}

void FileReader::close() {
    if (file != nullptr) {
        if (file->is_open()) {
            file->close();
        }
        delete file;
        file = nullptr;
    }
}

bool FileReader::hasNext() {
    return (file != nullptr && file->peek() != EOF);
}

string FileReader::next() {
    string line;
    if (file != nullptr && getline(*file, line)) {
        return line;
    }
    return "";
}

void FileReader::reset() {
    if (file != nullptr && file->is_open()) {
        file->clear();
        file->seekg(0, ios::beg);
    }
}
