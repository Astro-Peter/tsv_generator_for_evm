#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

struct CodeAndDescription{
    std::string code;
    std::string description;
};

class Command {
public:
    std::string command_name;
    std::string addres;
    static std::map<std::string, CodeAndDescription> commands;

    explicit Command(std::string& input);
    void WriteToFile(std::fstream& output) const;
};

std::map<std::string, CodeAndDescription> Command::commands = {{"ISZ", {"0", "(M) "
                                                                             "+ 1 \uF0E8 M, если (M) >= 0, "
                                                                             "то (CK) + 1 \uF0E8 CK"} },
                                                                {"AND", {"1", "(M) & (A) \uF0E8 A"}},
                                                                {"JSR", {"2", "(CK) \uF0E8 M, M + 1 \uF0E8 CK"}},
                                                                {"MOV", {"3", "(A) \uF0E8 M"}},
                                                                {"ADD", {"4", "(M) + (A) \uF0E8 A"}},
                                                                {"ADC", {"5", "(M) + (A) + (C) \uF0E8 A"}},
                                                                {"SUB", {"6", "(A) – (M) \uF0E8 A"}},
                                                                {"BCS", {"8", "Если (C) = 1, то M \uF0E8 CK"}},
                                                                {"BPL", {"9", "Если (A) >= 0, то M \uF0E8 CK"}},
                                                                {"BMI", {"A", "Если (A) < 0, то M \uF0E8 CK"}},
                                                                {"BEQ", {"B", "Если (A)и(C) = 0, то M \uF0E8 CK"}},
                                                                {"BR", {"C", "M \uF0E8 CK"}},
                                                                {"CLA", {"F200", "0 \uF0E8 A"}},
                                                                {"CLC", {"F300", "0 \uF0E8 C"}},
                                                                {"CMA", {"F400", "(!A) \uF0E8 A"}},
                                                                {"CMC", {"F500", "(!C) \uF0E8 C"}},
                                                                {"ROL", {"F600", "Содержимое A и C сдвигается "
                                                                                 "влево, А(15) \uF0E8 C, C \uF0E8 A(0)"}},
                                                                {"ROR", {"F700", "Содержимое A и C сдвигается "
                                                                                 "вправо, A(0) \uF0E8 C, C \uF0E8 A(15)"}},
                                                                {"INC", {"F800", "(A) + 1 \uF0E8 A"}},
                                                                {"DEC", {"F900", "(A) – 1 \uF0E8 A"}},
                                                                {"HLT", {"F000", "Остановка"}},
                                                                {"NOP", {"F100", "Нет операции"}},
                                                                {"EI", {"FA00", "Разрешение прерывания"}},
                                                                {"DI", {"FB00", "Запрещение прерывания"}},
                                                                {"CLF", {"E0", "0 \uF0E8 флаг устр. B"}},
                                                                {"TSF", {"E1", "Если (флаг устр. B) = 1,\n"
                                                                               "то (CK) + 1 \uF0E8 CK"}},
                                                                {"IN", {"E2", "(B) \uF0E8 A"}},
                                                                {"OUT", {"E3", "(A) \uF0E8 B"}}

};

Command::Command(std::string& input) {
    size_t delim = input.find(' ');
    command_name = input.substr(0, delim);
    if (commands[command_name].code.size() < 2) {
        addres = input.substr(delim + 1, input.size() - delim);
    } else {
        addres = "";
    }
}

void Command::WriteToFile(std::fstream& output) const {
    output << commands[command_name].code << addres << '\t';
    output << command_name << ' ' << addres << '\t';
    output << commands[command_name].description << '\n';
}

int main() {
    std::string table_top {"\"Адрес\"\t"
                           "\"Код команды\"\t"
                           "\"Мнемоника\"\t"
                           "\"Комментарии\"\n"};
    std::cout << "input file:\n";
    std::string filename;
    std::cin >> filename;
    std::fstream input(filename, std::fstream::in);
    std::cout << "output .tsv file:\n";
    std::cin >> filename;
    std::fstream output(filename, std::fstream::out);
    int start;
    std::string no;
    std::getline(input, no, ' ');
    input >> std::hex >> start;
    input.ignore(1);
    std::vector<Command> comms;
    while (input.good()){
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
