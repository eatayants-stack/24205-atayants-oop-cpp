#include <iostream>
#include <string>
#include <vector>


#include "FileReader.h"
#include "Parser.h"
#include "WordCounter.h"
#include "StatisticsCollector.h"
#include "CsvWriter.h"

using namespace std;


int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input.txt> <output.csv>" << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];


    FileReader reader(inputFilename);
    WordCounter counter;


    reader.open();
    while (reader.hasNext()) {
        string line = reader.next();

        vector<string> words = Parser::parseLine(line);
        counter.addWords(words);
    }
    reader.close();


    StatisticsCollector stats;
    stats.process(counter);


    CsvWriter writer(outputFilename);
    writer.write(stats);

    cout << "Saved to: " << outputFilename << endl;

    return 0;
}
