//
// Created by Vanst on 09.03.2023.
//

#ifndef UNTITLED12_COMMANDS_COMMAND_H_
#define UNTITLED12_COMMANDS_COMMAND_H_
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

struct CodeAndDescription {
    std::string code_;
    std::string description_;
};

class Command {
private:
    std::string command_name_;
    std::string address_;
    bool command_ = false;
    bool skip_ = false;
    bool pointer_ = false;
    static std::map<std::string, CodeAndDescription> commands;
public:
    explicit Command(std::string& input);
    void WriteToFile(std::fstream& output, size_t& start, size_t i, std::vector<std::string>& addresses) const;
};



#endif //UNTITLED12_COMMANDS_COMMAND_H_
