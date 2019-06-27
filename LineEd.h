//
// Created by pooja on 05/06/19.
//

#ifndef ASS2_LINEED_H
#define ASS2_LINEED_H

#include <fstream>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

using namespace std;
#include "Command.h"
class LineEd {
private:
    int countLines( ifstream&);
    bool askYesNo(const string & question);
    int addToBuffer(const string& in);
    void printBuffer(Command& )const;
    void bufferToFile(const string & filename);
    bool inputMode();
public:
    string filename;
    string input;
    int totalLines;
    int currentLine;
    void run();
    list<string> buffer;
    vector<string> clipboard;
    LineEd(const string& );

    bool runCommand(const string &);
};
#endif //ASS2_LINEED_H
