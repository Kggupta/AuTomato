#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

#include "Token.h"
#include "Expression.h"
#include "Statement.h"
#include "ParseHelper.h"
#include "Error.h"

using namespace std;

class Parser {
    public:
        explicit Parser(vector<Token> tokens);
        Token consume();
        Token consume(Token::Type expected, const string& expectedStr = string(""));

        shared_ptr<Expression> getPrefixExpression(const Token& tok);
        shared_ptr<Expression> getInfixExpression(const shared_ptr<Expression>& left, const Token& tok);
        shared_ptr<Expression> parseExpression(int precedence);
        shared_ptr<Expression> parseExpression();

        shared_ptr<statement> parseStatement();

        shared_ptr<statement> parse();

        Token lookAhead(unsigned int distance);
        int getPrecedence();
    private:
        int cur = 0;
        vector<Token> tokens;
        vector<Token> mRead;
};

#endif
