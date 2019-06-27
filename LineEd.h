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
#include <vector>
#include <string>

using namespace std;
#include "Command.h"
class LineEd {
private:
    int countLines( const stringstream&);
    bool askYesNo(const string& question);
    int addToBuffer(const string& in);
    void printBuffer(Command& ) const;
    void bufferToFile(const string& filename);
    bool inputMode();
    bool inputAt(const int& x);
    void u(Command&);
    void n(Command &cmd);
    void g(Command &cmd);
    void d(const int& x, const int & y);
    void c(const int &x, const int &y);
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
