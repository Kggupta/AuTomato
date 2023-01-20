#include "Error.h"

#include <utility>

using namespace std;

const char* Error::what() const noexcept {
    return message.c_str();
}

void Error::makeMessage(const string& cause, const string& details) {
    message = "File " + position.file + " | " + 
			  "Line " + to_string(position.line) + " | " +  
			  "Col " + to_string(position.column) + 
   			  " : " + cause + ": \n- "+ details;
}

Error::Error(const string& cause, const string& details) : position(Position(0,0,0)) {
    makeMessage(cause, details);
};

Error::Error(Position position, const string& cause) : position(move(position)) {
    makeMessage(cause, "");
};

Error::Error(Position position, const string& cause, const string& details) : position(move(position)) {
    makeMessage(cause, details);
};



const char* RunTimeError::what() const noexcept {
    return message.c_str();
}

RunTimeError::RunTimeError(shared_ptr<Context> context_, Position position, const string& cause, const string& details) 
	: Error(move(position), move(cause), move(details)), context(move(context_))
{   
    Context tmp_ctx = *context;
    Position tmp_pos = position;
    while (true) {
        message = "   File " +position.file + " | " + 
				  "Line " + to_string(tmp_pos.line) + " | " + 
				  "In: " + tmp_ctx.name + 
				  "\n" + message;
        if (!tmp_ctx.parent) break;
        tmp_ctx = *tmp_ctx.parent;
        tmp_pos = tmp_ctx.entry_pos;
    };
    message = cause + ": "+ details + "\nTraceback most recent call last: \n" + message;
};



IllegalCharacterException::IllegalCharacterException(Position position, const string& details) : Error(move(position), "IllegalCharacterException", move(details)) {}



ParseException::ParseException(Position position, const string& details) : Error(move(position), "ParseException", move(details)) {}



SyntaxError::SyntaxError(const string& details) : Error("SyntaxError", move(details)) {};

SyntaxError::SyntaxError(Position position, const string& details) : Error(move(position), "SyntaxError", move(details)) {};



TypeException::TypeException(Position position, const string& details) : Error(move(position), "TypeException", move(details)) {};



UndefinedOperationException::UndefinedOperationException(const string& t1, const string& t2) 
	: Error("UndefinedOperationException", "Between " + t1 + " and " + t2) {}

UndefinedOperationException::UndefinedOperationException(Position position, const string& t1, const string& op, const string& t2) 
	: Error(move(position), "UndefinedOperationException", "No operator " + op + " between " + t1 + " and " + t2) {}

UndefinedOperationException::UndefinedOperationException(Position position, const string& op, const string& t1) 
	: Error(move(position), "UndefinedOperationException", "No operator " + op + " and " + t1) {}

UndefinedOperationException::UndefinedOperationException(Position position, const string& op) 
	: Error(move(position), "UndefinedOperationException", "Unknown Operator Found: " + op) {}        



UndefinedVariable::UndefinedVariable(shared_ptr<Context> context, const string& name, Position position)
	: RunTimeError(context, move(position), "UndefinedVariable", name + " is not defined") {}



DivisionByZero::DivisionByZero(shared_ptr<Context> context, Position position) 
	: RunTimeError(move(context), move(position), "DivisionByZero", "Attempted division by zero") {}



IndexOutOfBounds::IndexOutOfBounds(shared_ptr<Context> context, Position position, const string& details)
	: RunTimeError(move(context), move(position), "IndexOutOfBounds", move(details)) {}
