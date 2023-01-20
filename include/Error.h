#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <exception>
#include <memory>

#include "Position.h"
#include "Context.h"

using namespace std;

struct Context;

class Error : public exception {
    public:
        string message;
        Position position;
        void makeMessage(const string& cause, const string& details);
        Error() = default;
        Error (const string& cause, const string& details);
        Error (Position position, const string& cause);
        Error (Position position, const string& cause, const string& details);
        virtual const char* what() const throw() override;
};

class RunTimeError : public Error {
    public:
        RunTimeError (shared_ptr<Context> context, Position pos, const string& cause, const string& details);
        virtual const char* what() const throw() override;
    protected:
        shared_ptr<Context> context;
};

class IllegalCharacterException : public Error {
    public:
        IllegalCharacterException(Position pos, const string& details);
};

class ParseException : public Error {
    public:
        ParseException(Position pos, const string& details);
};

class SyntaxError : public Error {
    public:
        SyntaxError(const string& details);
        SyntaxError(Position pos, const string& details);
};

class TypeException: public Error {
    public:
        TypeException(Position pos, const string& details);
};

class UndefinedOperationException : public Error {
    public:
        UndefinedOperationException(const string& t1, const string& t2);
        UndefinedOperationException(Position pos, const string& op);
        UndefinedOperationException(Position pos, const string& t1, const string& op);
        UndefinedOperationException(Position pos, const string& t1, const string& op, const string& t2);
};



class UndefinedVariable : public RunTimeError {
    public:
        UndefinedVariable(shared_ptr<Context> context, const string& name, Position pos);
};

class DivisionByZero : public RunTimeError {
    public:
        DivisionByZero(shared_ptr<Context> context, Position pos);
};

class IndexOutOfBounds : public RunTimeError {
    public:
        IndexOutOfBounds(shared_ptr<Context> context, Position pos, const string& details);
};

#endif
