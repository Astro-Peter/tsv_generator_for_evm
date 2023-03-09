//
// Created by Vanst on 09.03.2023.
//

#include "Command.h"

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
    command_name_ = input.substr(0, delim);
    if (commands.find(command_name_) != commands.end()){
        command_ = true;
    } else if (command_name_ == "$pos") {
        address_ = input.substr(delim + 1, input.size() - delim);
        skip_ = true;
        return;
    }
    if (commands[command_name_].code_.size() <= 2) {
        address_ = input.substr(delim + 1, input.size() - delim);
        if (address_.size() == 5) {
            address_ = address_.substr(1, 3);
            pointer_ = true;
        }
    } else {
        address_ = "";
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

void Command::WriteToFile(std::fstream& output, size_t& start, size_t i, std::vector<std::string>& addresses) const {
    std::ostringstream comm_address;
    comm_address << std::uppercase << std::hex << start + i;
    std::string address = comm_address.str();
    if (address.size() == 2){
        address = "0" + address;
    } else if (address.size() == 1){
        address = "00" + address;
    }
    if (command_ && (std::find(addresses.begin(), addresses.end(), address) != addresses.end())){
        output << address << '\t';
        output << commands[command_name_].code_ << address_ << '\t';
        output << command_name_ << ' ' << (pointer_ ? ('(' + address_ + ')') : address_) << '\t';
        std::string tmp = commands[command_name_].description_;
        if (address_.size() == 3) {
            Substitute(tmp, address_, 'M');
            if (pointer_) {
                tmp += " Косвенный доступ";
            }
        } else if (address_.size() == 2) {
            Substitute(tmp, address_, 'B');
        }
        output << tmp << '\n';
    } else if (skip_){
        start = std::stoll(address_, nullptr, 16) - i - 1;
    } else {
        output << address << '\t';
        output << command_name_ << '\n';
    }
}