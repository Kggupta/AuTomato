#ifndef LEXER_H
#define LEXER_H

#include <string> 
#include <vector>

#include "Token.h"
#include "Error.h"
#include "Position.h"

using namespace std;

class Lexer {
    public:
        Lexer(string fileName, const char* text);
        vector<Token> getTokens();
        Token advance();
        bool isIdentifier(char c);
        bool isSpace(char c);
        Token makeNumber();
        Token makeString();
        Token makeIdentifier();
        void skipComment();            
        const char* cur = nullptr;
    private:
        vector<Error> errors;
        Position position {0,0,0};
        char peek() const { return *cur;}
        char get() { 
            position.advance(*cur++);
            return *cur; 
        }
        char getLast() {
            char a = *cur;
            get();
            return a;
        }
};

#endif
