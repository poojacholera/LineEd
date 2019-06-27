//
// Created by pooja on 05/06/19.
//

#ifndef ASS2_COMMAND_H
#define ASS2_COMMAND_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

class Command {
private:
    string commandLine;
    bool status;
    string symbol;
    string address1;
    string address2;
    bool isValidCommand(const char &ch);
    bool isValidDigit(string &address);
    void compareAddresses();
    void swapAddresses();
public:
    Command(const string& command_line);
    void parse(const string& command_line);
    string getSymbol();
    string getAddress1();
    string getAddress2();
    bool getStatus();
    void setStatus(bool status);
};

#endif //ASS2_COMMAND_H
