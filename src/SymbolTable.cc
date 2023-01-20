#include "SymbolTable.h"
#include <utility>
#include "Statement.h"

using namespace std;

SymbolTable::SymbolTable(shared_ptr<SymbolTable> parent) : parent(move(parent)) {}

optional<sharedObj> SymbolTable::get(const string& name) {
    auto it = symbols.find(name);
    if (it != symbols.end()) return it->second;
    else if (it == symbols.end() && parent) {
        return parent->get(name);
    }
    return {};
}

void SymbolTable::set(const string& name, sharedObj value) {
    symbols[name] = move(value);
}

void SymbolTable::set(const string& name, Result val) {
    symbols[name] = make_shared<object>(val);
}

bool SymbolTable::update(const string& name, const sharedObj& value) {
    if (symbols.find(name) != symbols.end()) {
        set(name, value);
        return true;
    } else if (parent) {
        return parent->update(name, value);
    }
    return false;
}

void SymbolTable::remove(const string& name) {
    symbols.erase(name);
}

typedef variant<string, int, double, vector<shared_ptr<object>>, identifier, null> type;

void SymbolTable::addFunction(const string& name, const natFun& func, int numParams) {
    sharedObj t = make_shared<nativeFunction>(name, func, numParams);
    set(name, t);
}
