#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

struct CodeAndDescription {
    std::string code;
    std::string description;
};

class Command {
public:
    std::string command_name;
    std::string addres;
    bool pointer = false;
    static std::map<std::string, CodeAndDescription> commands;

    explicit Command(std::string& input);
    void WriteToFile(std::fstream& output) const;
};

std::map<std::string, CodeAndDescription> Command::commands = {{"ISZ", {"0", "(M) "
                                                                             "+ 1 -> M, если (M) >= 0, "
                                                                             "то (CK) + 1 -> CK"}},
                                                               {"AND", {"1", "(M) & (A) -> A"}},
                                                               {"JSR", {"2", "(CK) -> M, M + 1 -> CK"}},
                                                               {"MOV", {"3", "(A) -> M"}},
                                                               {"ADD", {"4", "(M) + (A) -> A"}},
                                                               {"ADC", {"5", "(M) + (A) + (C) -> A"}},
                                                               {"SUB", {"6", "(A) – (M) -> A"}},
                                                               {"BCS", {"8", "Если (C) = 1, то M -> CK"}},
                                                               {"BPL", {"9", "Если (A) >= 0, то M -> CK"}},
                                                               {"BMI", {"A", "Если (A) < 0, то M -> CK"}},
                                                               {"BEQ", {"B", "Если (A)и(C) = 0, то M -> CK"}},
                                                               {"BR", {"C", "M -> CK"}},
                                                               {"CLA", {"F200", "0 -> A"}},
                                                               {"CLC", {"F300", "0 -> C"}},
                                                               {"CMA", {"F400", "(!A) -> A"}},
                                                               {"CMC", {"F500", "(!C) -> C"}},
                                                               {"ROL", {"F600", "Содержимое A и C сдвигается "
                                                                                "влево, А(15) -> C, C -> A(0)"}},
                                                               {"ROR", {"F700", "Содержимое A и C сдвигается "
                                                                                "вправо, A(0) -> C, C -> A(15)"}},
                                                               {"INC", {"F800", "(A) + 1 -> A"}},
                                                               {"DEC", {"F900", "(A) – 1 -> A"}},
                                                               {"HLT", {"F000", "Остановка"}},
                                                               {"NOP", {"F100", "Нет операции"}},
                                                               {"EI", {"FA00", "Разрешение прерывания"}},
                                                               {"DI", {"FB00", "Запрещение прерывания"}},
                                                               {"CLF", {"E0", "0 -> флаг устр. B"}},
                                                               {"TSF", {"E1", "Если (флаг устр. B) = 1,\n"
                                                                              "то (CK) + 1 -> CK"}},
                                                               {"IN", {"E2", "(B) -> A"}},
                                                               {"OUT", {"E3", "(A) -> B"}}
};

Command::Command(std::string& input) {
    size_t delim = input.find(' ');
    command_name = input.substr(0, delim);
    if (commands[command_name].code.size() < 2) {
        addres = input.substr(delim + 1, input.size() - delim);
        if (addres.size() == 5) {
            addres = addres.substr(1, 3);
            pointer = true;
        }
    } else {
        addres = "";
    }
}

void Substitute(std::string& string, const std::string& address, char change) {
    std::string tmp = string;
    string = "";
    size_t ammount = std::count(tmp.begin(), tmp.end(), change);
    for (size_t i = 0; i < ammount; i++) {
        string.append(tmp.substr(0, tmp.find(change)) + address);
        tmp = tmp.substr(tmp.find(change) + 1, tmp.size() - tmp.find(change));
    }
    string += tmp;
};

void Command::WriteToFile(std::fstream& output) const {
    output << commands[command_name].code << addres << '\t';
    output << command_name << ' ' << addres << '\t';
    std::string tmp = commands[command_name].description;
    if (addres.size() == 3) {
        Substitute(tmp, addres, 'M');
        if (pointer) {
            tmp += " Косвенный доступ";
        }
    } else if (addres.size() == 2) {
        Substitute(tmp, addres, 'B');
    }
    //std::cout << tmp;
    output << tmp << '\n';
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
    int start;
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
        output << std::hex << i + start << '\t';
        comms[i].WriteToFile(output);
    }
    return 0;
}
