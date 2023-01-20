#include "types.h"
#include "Error.h"
#include <memory>
#include <utility>

using namespace std;
typedef  shared_ptr<object> sharedObj;

#define NO_OP throw UndefinedOperationException(value.getStringType(), obj->value.getStringType());
#define NO_UNARY(NAME) throw UndefinedOperationException(NAME, value.getStringType());
#define CREATE_OBJ(ARG, ret) make_shared<ret>(ARG)

object::object() : value(0) {}
object::object(Result res) : value(move(res)) {}

sharedObj object::addBy(const sharedObj& obj) {
    NO_OP
}

sharedObj object::subBy(const sharedObj& obj) {
    NO_OP
}

sharedObj object::multBy(const sharedObj& obj) {
    NO_OP
}

sharedObj object::divBy(const sharedObj& obj) {
    NO_OP
}

sharedObj object::powBy(const sharedObj& obj) {
    NO_OP
}

sharedObj object::mod(const sharedObj& obj) {
    NO_OP
}

sharedObj object::toBool() {
    CREATE_OBJ(value ? 1 : 0, intType);
}

sharedObj object::gt(const sharedObj& obj) {
    NO_OP
}

sharedObj object::lt(const sharedObj& obj) {
    NO_OP
}

sharedObj object::gte(const sharedObj& obj) {
    NO_OP
}

sharedObj object::lte(const sharedObj& obj) {
    NO_OP
}

sharedObj object::ee(const sharedObj& obj) {
    NO_OP
}

sharedObj object::ne(const sharedObj& obj) {
    NO_OP
}

sharedObj object::band(const sharedObj& obj) {
    NO_OP
}

sharedObj object::bor(const sharedObj& obj) {
    NO_OP
}

sharedObj object::bxor(const sharedObj& obj) {
    NO_OP
}

sharedObj object::lshift(const sharedObj& obj) {
    NO_OP
}

sharedObj object::rshift(const sharedObj& obj) {
    NO_OP
}

sharedObj object::index(const sharedObj& obj) {
    NO_OP
}

sharedObj object::indexUpdate(const sharedObj& obj, const sharedObj& newObj) {
    NO_OP
}

sharedObj object::bnot() {
    NO_UNARY("~")
}

sharedObj object::inc() {
    NO_UNARY("++")
}

sharedObj object::dec() {
    NO_UNARY("--")
}

sharedObj object::prePlus() {
    NO_UNARY("+")
}

sharedObj object::preMinus() {
    NO_UNARY("-")
}

sharedObj object::preNot() {
    return CREATE_OBJ(!toBool()->value, intType);
}

sharedObj object::call(Context& context, vector<sharedObj> args){
    throw UndefinedOperationException(value.getStringType(), "()");
}

string object::toString() {
    return "<object>";
}

sharedObj object::dot(const string& name, const vector<sharedObj>& args) {
    throw UndefinedOperationException(value.getStringType(), ".");
}

#define BOOL_NUMBER_OP(t, op) visit(overloaded{\
            [&](int arg) {ans = CREATE_OBJ(getValue<t>() op arg, intType);},\
            [&](double arg) {ans = CREATE_OBJ(getValue<t>() op arg, intType);},\
            [&](auto arg) {NO_OP}\
        }, obj->value.getResult());

intType::intType(int val) : object(Result(val)) {}

sharedObj intType::addBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](const string& arg) {ans = CREATE_OBJ(to_string(getValue<int>()).append(arg), stringType);},
        [&](int arg) {ans = CREATE_OBJ(getValue<int>() + arg, intType);},
        [&](double arg) {ans = CREATE_OBJ(((double) getValue<int>()) + arg, doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj intType::subBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](int arg) {ans = CREATE_OBJ(getValue<int>() - arg, intType);},
        [&](double arg) {ans = CREATE_OBJ(((double) getValue<int>()) - arg, doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj intType::multBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](const string& arg) {
                string finalString = arg;
                for (unsigned int i = 1; i < getValue<int>(); i++) {
                    finalString += arg;
                }
                ans = CREATE_OBJ(finalString, stringType);
            },
        [&](int arg) {ans = CREATE_OBJ(getValue<int>() * arg, intType);},
        [&](double arg) {ans = CREATE_OBJ(((double) getValue<int>()) * arg, doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj intType::divBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](int arg) {ans = CREATE_OBJ(getValue<int>() / arg, intType);},
        [&](double arg) {ans = CREATE_OBJ(((double) getValue<int>()) / arg, doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj intType::powBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](int arg) {ans = CREATE_OBJ((int) pow(getValue<int>(), arg), intType);},
        [&](double arg) {ans = CREATE_OBJ(pow(getValue<int>(), arg), doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj intType::mod(const sharedObj& obj) {
    if (!obj->value.isType<int>()) NO_OP
    return CREATE_OBJ(getValue<int>() % obj->getValue<int>(), intType);
}

sharedObj intType::ee(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(int, ==)
    if (ans) return ans;
    NO_OP
}

sharedObj intType::ne(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(int, !=)
    if (ans) return ans;
    NO_OP
}

sharedObj intType::gt(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(int, >)
    if (ans) return ans;
    NO_OP
}

sharedObj intType::lt(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(int, <)
    if (ans) return ans;
    NO_OP
}

sharedObj intType::gte(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(int, >=)
    if (ans) return ans;
    NO_OP
}

sharedObj intType::lte(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(int, <=)
    if (ans) return ans;
    NO_OP
}

#define BINARY_OP_INT(op) sharedObj ans = nullptr;\
    if (obj->value.isType<int>()) {\
        ans = CREATE_OBJ(getValue<int>() op obj->getValue<int>(), intType);\
    }\
    if (ans) return ans;\
    NO_OP

sharedObj intType::band(const sharedObj& obj) {
    BINARY_OP_INT(&)
}

sharedObj intType::bor(const sharedObj& obj) {
    BINARY_OP_INT(|)
}

sharedObj intType::bxor(const sharedObj& obj) {
    BINARY_OP_INT(^)
}

sharedObj intType::lshift(const sharedObj &obj) {
    BINARY_OP_INT(<<)
}

sharedObj intType::rshift(const sharedObj& obj) {
    BINARY_OP_INT(>>)
}

#undef BINARY_OP_INT

sharedObj intType::inc() {
    return CREATE_OBJ(getValue<int>() + 1, intType);
}

sharedObj intType::dec() {
    return CREATE_OBJ(getValue<int>() - 1, intType);
}

sharedObj intType::prePlus() {
    return CREATE_OBJ(getValue<int>(), intType);
}

sharedObj intType::preMinus() {
    return CREATE_OBJ(getValue<int>() * -1, intType);
}

sharedObj intType::bnot() {
    return CREATE_OBJ(~getValue<int>(), intType);
}

sharedObj intType::toBool() {
    return CREATE_OBJ(getValue<int>() != 0, intType);
}

string intType::toString() {
    return to_string(getValue<int>());
}

#define DOUBLE_BIN_OP(op) visit(overloaded{\
        [&](int arg) {ans = CREATE_OBJ(getValue<double>() op ((double) arg), doubleType);},\
        [&](double arg) {ans = CREATE_OBJ(getValue<double>() op arg, doubleType);},\
        [&](auto arg) {NO_OP}\
    }, obj->value.getResult());

doubleType::doubleType(double val) : object(Result(val)) {}

sharedObj doubleType::addBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](string& arg) {ans = CREATE_OBJ(to_string(getValue<double>()).append(arg), stringType);},
        [&](int arg) {ans = CREATE_OBJ(getValue<double>() + ((double) arg), doubleType);},
        [&](double arg) {ans = CREATE_OBJ(getValue<double>() + arg, doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::subBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    DOUBLE_BIN_OP(-)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::multBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    DOUBLE_BIN_OP(*)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::divBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    DOUBLE_BIN_OP(/)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::powBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](int arg) {ans = CREATE_OBJ(pow(getValue<double>(), (double) arg), doubleType);},
        [&](double arg) {ans = CREATE_OBJ((pow(getValue<double>(), arg)), doubleType);},
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::inc() {
    return CREATE_OBJ(getValue<double>() + 1, doubleType);
}

sharedObj doubleType::dec() {
    return CREATE_OBJ(getValue<double>() - 1, doubleType);
}

sharedObj doubleType::toBool() {
    return CREATE_OBJ(getValue<double>() != 0, doubleType);
}

sharedObj doubleType::ee(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(double, ==)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::ne(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(double, !=)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::gt(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(double, >)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::lt(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(double, <)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::gte(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(double, >=)
    if (ans) return ans;
    NO_OP
}

sharedObj doubleType::lte(const sharedObj& obj) {
    sharedObj ans = nullptr;
    BOOL_NUMBER_OP(double, <=)
    if (ans) return ans;
    NO_OP
}

string doubleType::toString() {
    string str = to_string(getValue<double>());
    str.erase ( str.find_last_not_of('0') + 1, string::npos );
    if (str.back() == '.') str.push_back('0');
    return str;
}

#undef DOUBLE_BIN_OP
#undef BOOL_NUMBER_OP

stringType::stringType(string val) : object(Result(val)) {}

sharedObj stringType::addBy(const sharedObj& obj) {
    return CREATE_OBJ(getValue<string>().append(obj->toString()), stringType);
}

sharedObj stringType::multBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](int arg) {
                auto finalString = getValue<string>();
                for (unsigned int i = 1; i < arg; i++) {
                    finalString += getValue<string>();
                }
                ans = CREATE_OBJ(finalString, stringType);
            },
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj stringType::toBool() {
    return CREATE_OBJ(!getValue<string>().empty(), intType);
}

string stringType::toString() {
    return getValue<string>();
}

sharedObj stringType::dot(const string& name, const vector<sharedObj>& args) {
    if (name == "length" || name == "len" || name == "size") {
        if (!args.empty()) throw Error("Function Call Exception", "Too many Params for " + name + ": expected, " + to_string(0) + " params but called with " + to_string(args.size()));
        return CREATE_OBJ(getValue<string>().length(), intType);
    }
    throw Error("Undefined Method", value.getStringType() + " has no member function " + name);
}

sharedObj stringType::index(const sharedObj& obj) {
    if (!obj->value.isType<int>()) NO_OP;
    int toAccess = obj->getValue<int>();
    int size = getValue<string>().length();
    if (toAccess >=  size || toAccess < 0) {
        throw Error("Index Out of Bounds", "Attempted to access index: " + to_string(toAccess) + ", with string size: " + to_string(size));
    }
    return CREATE_OBJ(string(1, getValue<string>()[toAccess]), stringType);
}


listType::listType(vector<sharedObj> val) : object(Result(val)) {}

sharedObj listType::addBy(const sharedObj& obj) {
    sharedObj ans = nullptr;
    visit(overloaded{
        [&](vector<sharedObj> arg) {
            vector<sharedObj> finalArray;
            finalArray.reserve(getValue<vector<sharedObj>>().size());
            for (const auto & i : getValue<vector<sharedObj>>()) {
                finalArray.push_back(i);
            }
            for (const auto & i : arg) {
                finalArray.push_back(i);
            }
            ans = CREATE_OBJ(finalArray, listType);
            },
        [&](auto arg) {NO_OP}
    }, obj->value.getResult());
    if (ans) return ans;
    NO_OP
}

sharedObj listType::multBy(const sharedObj& obj) {
    NO_OP
}

sharedObj listType::toBool() {
    return CREATE_OBJ(getValue<vector<sharedObj>>().empty(), intType);
}

sharedObj listType::index(const sharedObj& obj) {
    if (!obj->value.isType<int>()) NO_OP
    int toAccess = obj->getValue<int>();
    int size = getValue<vector<sharedObj>>().size();
    if (toAccess >=  size || toAccess < 0) {
        throw Error("Index Out of Bounds", "Attempted to access index: " + to_string(toAccess) + ", with list size: " + to_string(size));
    }
    return getValue<vector<sharedObj>>()[toAccess];
}

sharedObj listType::indexUpdate(const sharedObj& obj, const sharedObj& newObj) {
    if (!obj->value.isType<int>()) NO_OP
    int toAccess = obj->getValue<int>();
    int size = getValue<vector<sharedObj>>().size();
    if (toAccess >=  size || toAccess < 0) {
        throw Error("Index Out of Bounds", "Attempted to access index: " + to_string(toAccess) + ", with list size: " + to_string(size));
    }
    getValue<vector<sharedObj>>()[toAccess] = newObj;
    return make_shared<nullType>();
}

string listType::toString() {
    auto values = getValue<vector<sharedObj>>();
    string str("[");
    for (unsigned int i = 0 ; i < values.size(); i++) {
        if (values[i]) str += values[i]->toString();
        else str += "null";
        if (i != values.size() - 1) str += ", ";
    }
    str.push_back(']');
    return str;
}

sharedObj listType::dot(const string &name, const vector<sharedObj> &args) {
    if (name == "size" || name == "length" || name == "len") {
        return CREATE_OBJ(getValue<vector<sharedObj>>().size(), intType);
    } else if (name == "add" && args.size() == 1) {
        getValue<vector<sharedObj>>().push_back(args[0]);
        return make_shared<nullType>();
    }
    throw Error("Undefined Method", value.getStringType() + " has no member function " + name);
}

functionType::functionType (string name, vector<string> params, shared_ptr<statement> toRun) 
: object(Result(identifier())), name(move(name)), params(move(params)), toRun(move(toRun)) {}
sharedObj functionType::call(Context& parent, vector<sharedObj> args) {
    if (args.size() < params.size()) {
        throw Error("Function Call Exception", "Too few Params for " + name + ": expected, " + to_string(params.size()) + " params but called with " + to_string(args.size()));
    }
    if (args.size() > params.size()) {
        throw Error("Function Call Exception", "Too many Params for " + name + ": expected, " + to_string(params.size()) + " params but called with " + to_string(args.size()));
    }
    shared_ptr<SymbolTable> symbols = make_shared<SymbolTable>(parent.symbols);
    for (unsigned int i = 0; i < params.size(); i++) {
        symbols->set(params[i], args[i]);
    }
    Context child(name, symbols);
    flow temp = toRun->execute(child);
    if (temp.value) return *temp.value;
    else return make_shared<nullType>();
}

string functionType::toString() {
    return "function <" + name + ">";
}

nullType::nullType() {
    value = Result(null());
}

string nullType::toString() {
    return "null";
}

nativeFunction::nativeFunction(string name_, function<optional<type>(vector<sharedObj>)>  func_, int numParams = -1) 
    : name(move(name_)), numberOfParams(numParams), toRun(move(func_)) {}

sharedObj nativeFunction::call(Context &context, vector<sharedObj> args) {
    if (numberOfParams != -1) {
        if (args.size() > numberOfParams) {
            throw Error("Function Call Exception", "Too many Params for " + name + ": expected, " + to_string(numberOfParams) + " params but called with " + to_string(args.size()));
        }
        else if (args.size() < numberOfParams) {
            throw Error("Function Call Exception", "Too few Params for " + name + ": expected, " + to_string(numberOfParams) + " params but called with " + to_string(args.size()));
        }
    }
    optional<type> got = toRun(args);
    if (!got) return make_shared<nullType>();
    sharedObj ret;
    visit(overloaded{
            [&](vector<sharedObj> arg) {
                ret = CREATE_OBJ(arg, listType);
            },
            [&](int arg) {
                ret = CREATE_OBJ(arg, intType);
            },
            [&](double arg) {
                ret = CREATE_OBJ(arg, doubleType);
            },
            [&](string arg) {
                ret = CREATE_OBJ(arg, stringType);
            },
            [&](null arg) {
                ret = make_shared<nullType>();
            },
            [&](auto arg) {
                throw Error("Unknown Error", "Unknown return type when running: " + name);
            }
    }, *got);
    return ret;
}


#undef CREATE_OBJ

