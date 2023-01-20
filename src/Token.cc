#include "Token.h"

#include <string>
#include <utility>

using namespace std;

string Token::getValue() const {
    return value;
}

bool Token::is(Type t) const {
    return this->type == t;
}

bool Token::isNot(Type t) const {
	return !is(t);
}

bool Token::isOneOf(Type t1, Type t2) const {
    return is(t1) || is(t2);
}

Token::Token(Type t, Position pos) 
    : type(t), startPos(move(pos)) {}

Token::Token(Type t, string value, Position pos) 
    : type(t), startPos(move(pos)), value(move(value)) {}

Token::Token(Type t, char value, Position pos) 
    : type(t), startPos(move(pos)), value(string(1, value)) {}

Token::Token(Type t, const char* start, const char* end, Position pos) 
    : type(t), startPos(move(pos)), value(string(start, end)) {}

Token::Token(Type t, const char* start, size_t length, Position pos) 
    : type(t), startPos(move(pos)), value(string(start, length)) {}
