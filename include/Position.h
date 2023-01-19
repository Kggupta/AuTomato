#ifndef POSITION_H
#define POSITION_H

#include <string>
#include <utility>

using namespace std;

class Position {
    public:
        Position() = default;;
        Position(int current, int line, int column);
        Position(int current, int line, int column, string file, string fcontent);
        Position* advance(char current);
        Position copy();
        
        // Copy constructor to replace copy()
        Position(const Position & rhs);


        int position = 0;
        int line = 0;
        int column = 0;

        string file;
        string fcontent;
};

#endif
