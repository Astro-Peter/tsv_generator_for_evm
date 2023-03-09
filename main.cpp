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
    std::vector<Command> comms;
    while (input.good()) {
        std::string tmp;
        std::getline(input, tmp, '\n');
        if (!tmp.empty()){
            Command comm(tmp);
            comms.push_back(comm);
        }
    }
    output << table_top;
    size_t start = 0;
    for (size_t i = 0; i < comms.size(); i++) {
        comms[i].WriteToFile(output, start, i);
    }
    return 0;
}
