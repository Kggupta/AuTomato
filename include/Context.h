#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <memory>
#include "Position.h"
#include "SymbolTable.h"

using namespace std;

class SymbolTable;
struct Result;

struct Context {
    Context() {};

    Context(string name, shared_ptr<SymbolTable> symbols) 
    : name(name), parent(nullptr), symbols(symbols) {}

    Context(string name, shared_ptr<Context> parent, shared_ptr<SymbolTable> symbols) 
    : name(name), parent(parent), symbols(symbols) {}

    Context(string name, shared_ptr<Context> parent, Position & entry_pos, shared_ptr<SymbolTable> symbols) 
    : name(name), entry_pos(entry_pos), parent(parent), symbols(symbols) {}

    string name;
    Position entry_pos;
    shared_ptr<Context> parent;
    shared_ptr<SymbolTable> symbols = nullptr;
};

#endif
