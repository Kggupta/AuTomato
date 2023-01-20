#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <optional>

#include "types.h"

struct object;
struct Result;
class identifier;
struct null;

using namespace std;

typedef shared_ptr<object> sharedObj;
typedef function<optional<variant<string, int, double, vector<shared_ptr<object>>, identifier, null>>(vector<sharedObj>)> natFun;

class SymbolTable {
    public:
        SymbolTable() {}
        SymbolTable(shared_ptr<SymbolTable> parent);
        shared_ptr<SymbolTable> parent = nullptr;
        bool update(const string& name, const sharedObj& value);
        optional<sharedObj> get(const string& name);
        void set(const string& name, sharedObj value);
        void set(const string& name, Result val);
        void remove(const string& name);
        void addFunction(const string& name, const natFun& func, int numParams);
    private:
        unordered_map<string, sharedObj> symbols;
};

#endif
