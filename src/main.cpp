#include "FileReader.h"
#include "CsvWriter.h"
#include "Parser.h"
#include "StatisticsCollector.h"
#include "WordCounter.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input> <output>\n";
        return 1;
    }

    FileReader reader(argv[1]);
    if (!reader.isOpen()) {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1;
    }

    Parser parser;
    WordCounter counter;

    while (!reader.isEOF()) {
        const std::string line = reader.getLine();
        const std::vector<std::string> words = parser.parseLine(line);
        counter.addWords(words);
    }

    CsvWriter writer(argv[2]);
    if (!writer.isOpen()) {
        std::cerr << "Error: Cannot open output file: " << argv[2] << "\n";
        return 1;
    }

    StatisticsCollector statsCollector;
    statsCollector.process(counter);
    const auto& stats = statsCollector.getResults();

    writer.write({"word", "frequency", "percentage"});
    for (const auto& wordStat : stats) {
        std::ostringstream freqStream;
        freqStream << std::fixed << std::setprecision(3) << wordStat.percentage;
        std::vector<std::string> row = {
            wordStat.word,
            std::to_string(wordStat.frequency),
            freqStream.str()
        };
        writer.write(row);
    }
}