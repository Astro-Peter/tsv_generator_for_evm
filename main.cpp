#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "command/Command.h"

std::vector<std::string> CheckComms(std::fstream& tracing, std::vector<Command>& comms) {
    std::vector<std::string> result;
    std::string header;
    std::getline(tracing, header, '\n');
    while (tracing.good()) {
        std::string tmp;
        std::getline(tracing, tmp, '\n');
        tmp = tmp.substr(1, 3);
        result.push_back(tmp);
    }
    return result;
}

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
    std::cout << "use tracing table?(y/n)\n";
    char let;
    std::vector<std::string> addresses;
    std::cin >> let;
    if (let == 'y') {
        std::cout << "input file:\n";
        std::cin >> filename;
        std::fstream tracing(filename, std::fstream::in);
        addresses = CheckComms(tracing, comms);
    }
    for (auto& i : addresses) {
        std::cout << i << '\n';
    }
    for (size_t i = 0; i < comms.size(); i++) {
        comms[i].WriteToFile(output, start, i, addresses);
    }
    return 0;
}
