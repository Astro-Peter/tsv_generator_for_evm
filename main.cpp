#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "command/Command.h"

int main() {
    std::string table_top{"\"Адрес\"\t"
                          "\"Код команды\"\t"
                          "\"Мнемоника\"\t"
                          "\"Комментарии\"\n"};
    std::cout << "input file:\n";
    std::string filename;
    std::cin >> filename;
    std::fstream input(filename, std::fstream::in);
    std::cout << "output .tsv file:\n";
    std::cin >> filename;
    std::fstream output(filename, std::fstream::out | std::fstream::trunc);
    size_t start;
    std::string no;
    std::getline(input, no, ' ');
    input >> std::hex >> start;
    input.ignore(1);
    std::vector<Command> comms;
    while (input.good()) {
        std::string tmp;
        std::getline(input, tmp, '\n');
        Command comm(tmp);
        comms.push_back(comm);
    }
    output << table_top;
    for (size_t i = 0; i < comms.size() - 1; i++) {
        comms[i].WriteToFile(output, start, i);
    }
    return 0;
}
