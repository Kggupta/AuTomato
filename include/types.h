#ifndef TYPES_H
#define TYPES_H

#include <variant>
#include <optional>
#include <memory>
#include <functional>

#include "Statement.h"
#include "Result.h"
#include "Context.h"

struct object;
class statement;

using namespace std;

typedef shared_ptr<object> sharedObj;

struct object {
    object();
    object(Result res);
    virtual sharedObj addBy(const sharedObj& obj);
    virtual sharedObj subBy(const sharedObj& obj);
    virtual sharedObj multBy(const sharedObj& obj);
    virtual sharedObj divBy(const sharedObj& obj);
    virtual sharedObj powBy(const sharedObj& obj);
    virtual sharedObj mod(const sharedObj& obj);
    virtual sharedObj toBool();
    virtual sharedObj gt(const sharedObj& obj);
    virtual sharedObj lt(const sharedObj& obj);
    virtual sharedObj gte(const sharedObj& obj);
    virtual sharedObj lte(const sharedObj& obj);
    virtual sharedObj ee(const sharedObj& obj);
    virtual sharedObj ne(const sharedObj& obj);
    virtual sharedObj band(const sharedObj& obj);
    virtual sharedObj bor(const sharedObj& obj);
    virtual sharedObj bxor(const sharedObj& obj);
    virtual sharedObj lshift(const sharedObj& obj);
    virtual sharedObj rshift(const sharedObj& obj);
    virtual sharedObj index(const sharedObj& obj);
    virtual sharedObj indexUpdate(const sharedObj& index, const sharedObj& newObj);
    virtual sharedObj bnot();
    virtual sharedObj inc();
    virtual sharedObj dec();
    virtual sharedObj prePlus();
    virtual sharedObj preMinus();
    virtual sharedObj preNot();
    virtual sharedObj dot(const string& name, const vector<sharedObj>& args);
    virtual sharedObj call(Context& context, vector<sharedObj> args);
    virtual string toString();

    template <typename T>
    T& getValue() {
        return value.getValue<T>();
    }
    virtual ~object() = default;
    Result value;
};

struct nullType : object {
    nullType();
    string toString() override;
};

struct intType : object {
    intType(int val);
    sharedObj addBy(const sharedObj& obj) override;
    sharedObj subBy(const sharedObj& obj) override;
    sharedObj multBy(const sharedObj& obj) override;
    sharedObj divBy(const sharedObj& obj) override;
    sharedObj mod(const sharedObj& obj) override;
    sharedObj powBy(const sharedObj& obj) override;
    sharedObj ee(const sharedObj& obj) override;
    sharedObj ne(const sharedObj& obj) override;
    sharedObj gt(const sharedObj& obj) override;
    sharedObj lt(const sharedObj& obj) override;
    sharedObj gte(const sharedObj& obj) override;
    sharedObj lte(const sharedObj& obj) override;
    sharedObj band(const sharedObj& obj) override;
    sharedObj bor(const sharedObj& obj) override;
    sharedObj bxor(const sharedObj& obj) override;
    sharedObj lshift(const sharedObj& obj) override;
    sharedObj rshift(const sharedObj& obj) override;
    sharedObj bnot() override;
    sharedObj inc() override;
    sharedObj dec() override;
    sharedObj prePlus() override;
    sharedObj preMinus() override;
    sharedObj toBool() override;

    string toString() override;

};
        
struct doubleType : object {
    doubleType(double val);
    sharedObj addBy(const sharedObj& obj) override;
    sharedObj subBy(const sharedObj& obj) override;
    sharedObj multBy(const sharedObj& obj) override;
    sharedObj divBy(const sharedObj& obj) override;
    sharedObj powBy(const sharedObj& obj) override;
    sharedObj inc() override;
    sharedObj dec() override;
    sharedObj toBool() override;
    sharedObj ee(const sharedObj& obj) override;
    sharedObj ne(const sharedObj& obj) override;
    sharedObj gt(const sharedObj& obj) override;
    sharedObj lt(const sharedObj& obj) override;
    sharedObj gte(const sharedObj& obj) override;
    sharedObj lte(const sharedObj& obj) override;

    string toString() override;
};

struct stringType : object {
    stringType(string val);
    sharedObj addBy(const sharedObj& obj) override;
    sharedObj multBy(const sharedObj& obj) override;
    sharedObj toBool() override;
    string toString() override;
    sharedObj index(const sharedObj& obj) override;
    sharedObj dot(const string& name, const vector<sharedObj>& args) override;
};

struct listType : object {
    listType(vector<sharedObj> val);
    sharedObj addBy(const sharedObj& obj) override;
    sharedObj multBy(const sharedObj& obj) override;
    sharedObj toBool() override;
    sharedObj index(const sharedObj& obj) override;
    sharedObj indexUpdate(const sharedObj& obj, const sharedObj& newObj) override;
    sharedObj dot(const string& name, const vector<sharedObj>& args) override;
    string toString() override;
};

struct functionType : object {
    functionType (string name, vector<string> params, shared_ptr<statement> toRun);
    sharedObj call(Context& context, vector<sharedObj> args) override;
    string toString() override;
    string name;
    vector<string> params; 
    shared_ptr<statement> toRun;
};

typedef variant<string, int, double, vector<shared_ptr<object>>, identifier, null> type;

struct nativeFunction : object {
    nativeFunction(string name_, function<optional<type>(vector<sharedObj>)>  func_, int numParams);
    sharedObj call(Context& context, vector<sharedObj> args) override;
    string name;
    int numberOfParams = 0;
    function<optional<type>(vector<sharedObj>)> toRun;
};

#endif
