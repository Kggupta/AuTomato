#include <vector>
#include "Lexer.h"
#include <string.h>
#include <exception>

using namespace std;

Lexer::Lexer(string fileName, const char* text) {
    cur = text;
    position = Position(0,0,0, fileName, string(text));
}

bool Lexer::isIdentifier(char c) {
    return isdigit(c) || isalnum(c) || c == '_';
}

bool Lexer::isSpace(char c) {
    return iswspace(c);
}

Token Lexer::makeIdentifier() {
    const char* start = cur;
    get();
    while (isIdentifier(peek())) get();
    string value(start, cur);
    if (value == "if") return Token(Token::Type::IF, value, position);
    else if(value == "elif") return Token(Token::Type::ELIF, value, position);
    else if (value == "else") return Token(Token::Type::ELSE, value, position);
    else if (value == "while") return Token(Token::Type::WHILE, value, position);
    else if (value == "for") return Token(Token::Type::FOR, value, position);
    else if (value == "in") return Token(Token::Type::IN, value, position); 
    else if (value == "var") return Token(Token::Type::VAR, value, position);
    else if (value == "fun") return Token(Token::Type::FUNCTION, value, position);
    else if (value == "true" || value == "false") return Token(Token::Type::BOOL, value, position);
    else if (value == "return") return Token(Token::Type::RET_T, value, position);
    else if (value == "break") return Token(Token::Type::BREAK, value, position);
    else if (value == "continue") return Token(Token::Type::CONTINUE, value, position);
    else if (value == "exit") return Token(Token::Type::EXIT, value, position);
    else return Token(Token::Type::IDENTIFIER, value, position);
}

Token Lexer::makeNumber() {
    const char* start = cur;
    int dotCount = 0;
    get();
    while (isdigit(peek()) || peek() == '.') {
        if (peek() == '.') dotCount++;
        if (dotCount > 1) break;
        get();
    }
    if (dotCount > 1) throw ParseException(position, "Could not parse expression: " + string(start, cur));
    else if (dotCount == 1) return Token(Token::Type::DOUBLE, start, cur, position);
    else return Token(Token::Type::INT, start, cur, position);
}

Token Lexer::makeString() {
    string val;
    while (peek() != '\"' && peek() != '\0') {
        val.push_back(peek());
        get();
    }
    if (peek() == '\0') throw ParseException(position, "Unexpected end of file: expected a closing '\"'");
    get();
    return Token(Token::Type::STRING, val, position);
}

void Lexer::skipComment() {
    while (peek() != '\n' && peek() != '\0') {
        get();
    }
}

Token Lexer::advance() {
    while(isSpace(peek())) get();
    switch (peek()) {
        case '\0':
            return Token(Token::Type::END, "EOF", position);
        case ';':
            return Token(Token::Type::STMT_END, getLast(), position);
        case '+': {
                char next = get();
                if (next == '+') {
                    get();
                    return Token(Token::Type::INC, "++", position);
                }
                else if (next == '=') {
                    get();
                    return Token(Token::Type::PLUS_EQUAL, "+=", position);
                }
                return Token(Token::Type::PLUS, '+', position);
            }
        case '-': {
                char next = get();
                if (next == '-') {
                    get();
                    return Token(Token::Type::DEC, "--", position);
                }
                else if (next == '=') {
                    get();
                    return Token(Token::Type::MINUS_EQUAL, "-=", position);
                }
                else if (next == '>') {
                    get();
                    return Token(Token::Type::ARROW, "->", position);
                }
                return Token(Token::Type::MINUS, '-', position);
            }
        case '*':
            if (get() == '=') {
                get();
                return Token(Token::Type::MULT_EQUAL, "*=", position);
            }
            return Token(Token::Type::MULT, '*', position);
        case '/': {
                char next = get();
                if (next == '=') {
                    get();
                    return Token(Token::Type::DIV_EQUAL, "/=", position);
                }
                else if (next == '/') {
                    get();
                    skipComment();
                    return advance();
                }
                return Token(Token::Type::DIV, '/', position);
            }
            case '%': {
                char next = get();
                if (next == '=') {
                    get();
                    return Token(Token::Type::MOD_EQUAL, "/=", position);
                }
                return Token(Token::Type::MOD, getLast(), position);
            }
        case '\"':
            get();
            return makeString();
        case '(':
            return Token(Token::Type::LPAREN, getLast(), position);
        case ')':
            return Token(Token::Type::RPAREN, getLast(), position);
        case '{':
            return Token(Token::Type::LCURL, getLast(), position);
        case '}':
            return Token(Token::Type::RCURL, getLast(), position);
        case '^':
            if (get() == '^') {
                get();
                return Token(Token::Type::POWER, "^^", position);
            }
            return Token(Token::Type::XOR, '^', position);
        case '[':
            return Token(Token::Type::LSQUARE, getLast(), position);
        case ']':
            return Token(Token::Type::RSQUARE, getLast(), position);
        case ',':
            return Token(Token::Type::COMMA, getLast(), position);
        case '=':
            if (get() == '=') {
                get();
                return Token(Token::Type::EE, "==", position);
            }
            return Token(Token::Type::EQUALS, '=', position);
        case '!':
            if (get() == '=') {
                get();
                return Token(Token::Type::NE, "!=", position);
            }
            return Token(Token::Type::NOT, '!', position);
        case '>': {
            char next = get();
            if (next == '=') {
                get();
                return Token(Token::Type::GE, ">=", position);
            } else if (next == '>') {
                get();
                return Token(Token::Type::RSHIFT, ">>", position);
            }
            return Token(Token::Type::GT, '>', position);
        }
        case '<': {
            char next = get();
            if (next == '=') {
                get();
                return Token(Token::Type::LE, "<=", position);
            }
            else if (next == '<') {
                get();
                return Token(Token::Type::LSHIFT, "<<", position);
            }
            return Token(Token::Type::LT, '<', position);
        }
        case '&':
            if (get() == '&') {
                get();
                return Token(Token::Type::AND, "&&", position);
            }
            return Token(Token::Type::BAND, '&', position);
        case '|':
            if (get() == '|') {
                get();
                return Token(Token::Type::OR, "||", position);
            }
            return Token(Token::Type::BOR, '|', position);
        case '.':
            return Token(Token::Type::DOT, getLast(), position);
        case '~':
            return Token(Token::Type::BNOT, getLast(), position);
        case ':':
            return Token(Token::Type::COLON, getLast(), position);
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
            return makeIdentifier();
        case '0' ... '9':
            return makeNumber();
        default:
            throw IllegalCharacterException(position, string("Unknown Character found while parsing: ") += peek());
    }
}

vector<Token> Lexer::getTokens() {
    vector<Token> tokens;
    Token next = advance();
    while (!next.is(Token::Type::END)) {
        tokens.push_back(next);
        next = advance();
    }
    tokens.push_back(next);
    return tokens;
}
