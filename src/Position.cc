#include "Position.h"

using namespace std;

Position::Position(int cur, int line, int column) : position(cur), line(line), column(column) {};

Position::Position(int cur, int line, int column, string file, string fcontent) 
                : position(cur), line(line), column(column), file(move(file)), fcontent(move(fcontent)) {};

Position * Position::advance(char current) {
    position++;
    column++;

    if (current == '\n') {
        line++;
        column = 0;
    }

    return this;
}

Position Position::copy() {
    return Position(position, line, column, file, fcontent);
}

Position::Position(const Position & rhs) {
	position = rhs.position;
	line = rhs.line;
	column = rhs.column;
	file = rhs.file;
	fcontent = rhs.fcontent;
}
