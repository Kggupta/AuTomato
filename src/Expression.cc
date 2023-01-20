#include <utility>
#include <Expression.h>


#include "Interpreter.h"
using namespace std;

Expression::Expression() = default;

string Expression::toString() {
    return "(null)";
}

sharedObj Expression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

Token Expression::getToken() { return tok; }

Expression::~Expression() = default;

PostfixExpression::PostfixExpression(shared_ptr<Expression> left, Token tok_) : left(move(left)){
    tok = tok_;
}

sharedObj PostfixExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

string PostfixExpression::toString() {
    return "(" + left->toString() + tok.getValue() + ")";
}

PrefixExpression::PrefixExpression(Token tok_, shared_ptr<Expression> right) : right(move(right)) {
    tok = move(tok_);
}

string PrefixExpression::toString() {
    return "(" + tok.getValue() + right->toString() + ")";
}

sharedObj PrefixExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

NameExpression::NameExpression(string name, Token tok_) : name(move(name)) {
    tok = tok_;
}

sharedObj NameExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

string NameExpression::toString() {
    return name;
}

NumberExpression::NumberExpression(const Token& t) {
    if (t.is(Token::Type::INT)) value = stoi(t.getValue());
    else if(t.is(Token::Type::DOUBLE)) value = stod(t.getValue());
    tok = t;
}

string NumberExpression::toString() {
    auto* str = get_if<int>(&value);
    if (str != nullptr) return to_string(*str);
    else return to_string(get<double>(value));
}

int NumberExpression::getInt() {
    auto* val = get_if<int>(&value);
    if (val == nullptr) return (int) get<double>(value);
    return *val;
}

double NumberExpression::getDouble() {
    return get<double>(value);
}

sharedObj NumberExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

StringExpression::StringExpression(const Token& t) {
    tok = t;
    value = t.getValue();
}

string StringExpression::toString() {
    return getValue();
}

string StringExpression::getValue() {
    return value;
}

sharedObj StringExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

ListExpression::ListExpression(Token t, vector<shared_ptr<Expression>> values) : value(move(values)) {
    tok = t;
}

string ListExpression::toString() {
    string str("[");
    for (unsigned int i = 0 ; i < value.size(); i++) {
        if (value[i]) str += value[i]->toString();
        else str += "null";
        if (i != value.size() - 1) str += ", ";
    }
    str.push_back(']');
    return str;
}

sharedObj ListExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

vector<shared_ptr<Expression>> ListExpression::getValue() { return value;}

BinOpExpression::BinOpExpression(shared_ptr<Expression> left, Token op, shared_ptr<Expression> right) 
    : left(move(left)), right(move(right)) { tok = op; }

string BinOpExpression::toString() {
    return "(" + left->toString() + tok.getValue()+ right->toString() + ")";
}

sharedObj BinOpExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

ComparisonExpression::ComparisonExpression(shared_ptr<Expression> left, Token op, shared_ptr<Expression> right) 
    :left(move(left)), right(move(right)) { tok = move(op); }

string ComparisonExpression::toString() {
    return "(" + left->toString() + tok.getValue() + right->toString() + ")";
}

sharedObj ComparisonExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

AssignmentExpression::AssignmentExpression(string name, shared_ptr<Expression> right, Token eq) 
    : name(move(name)), right(move(right)) { tok = move(eq); }

string AssignmentExpression::toString() {
    return "(" + name + tok.getValue()+ right->toString() + ")";
}

sharedObj AssignmentExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

UpdateExpression::UpdateExpression(string name, shared_ptr<Expression> right, Token eq)
    : name(move(name)), right(move(right)) { tok = move(eq); }

string UpdateExpression::toString() {
    return "(" + name + tok.getValue()+ right->toString() + ")";
}

sharedObj UpdateExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

ObjectIndexUpdateExpression::ObjectIndexUpdateExpression(
            const Token& tok, 
            shared_ptr<Expression> objToUpdate,
            shared_ptr<Expression> index, 
            shared_ptr<Expression> newVal) 
    : tok(tok), objToUpdate(objToUpdate), index(index), newVal(newVal) {}

sharedObj ObjectIndexUpdateExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

string ObjectIndexUpdateExpression::toString() {
    return tok.getValue() + index->toString() + "]" + " = " + newVal->toString();
}

//Conditional
ConditionalExpression::ConditionalExpression(
            Token tok_, 
            shared_ptr<Expression> condition_, 
            shared_ptr<Expression> then_, 
            vector<shared_ptr<Expression>> elif_conditions_, 
            vector<shared_ptr<Expression>> elif_thens_, 
            shared_ptr<Expression> elseBranch_)
: condition(move(condition_)),
    thenBranch(move(then_)),
    elif_conditions(move(elif_conditions_)),
    elif_thens(move(elif_thens_)),
    elseBranch(move(elseBranch_))
{ tok = move(tok_); }

sharedObj ConditionalExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

string ConditionalExpression::toString() {
    return "(if " + condition->toString() + ", then {" + thenBranch->toString() + "} else {" + elseBranch->toString() + "})";
}

CallFunctionExpression::CallFunctionExpression(
            shared_ptr<Expression> left_,
            Token tok_, 
            vector<shared_ptr<Expression>> params
        ) : left(move(left_)),params(move(params)) { tok = move(tok_);}

sharedObj CallFunctionExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
};

string CallFunctionExpression::toString() {
    string call = tok.getValue() + "(";
    for (unsigned int i = 0; i < params.size(); i++) {
        call += params[i]->toString();
        if (i != params.size() - 1) call.append(",");
    }
    call.append(")");
    return call;
};

IndexExpression::IndexExpression(
            shared_ptr<Expression> left_,
            Token tok_, 
            shared_ptr<Expression> index
        ) : index(move(index)), left(move(left_)) { tok = move(tok_); }

sharedObj IndexExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

string IndexExpression::toString() {
    return getToken().getValue() + "[" + index->toString() + "]";
}

FuncDefExpression::FuncDefExpression(
    Token tok_, 
    string name, 
    vector<string> params, 
    shared_ptr<statement> toRun,
    bool anonymous) 
: params(move(params)), name(move(name)), body(move(toRun)), lambda(anonymous) { tok = tok_; }

string FuncDefExpression::toString() {
    string ans = name + "(";
    for (unsigned int i = 0; i < params.size() -1; i++) ans += params[i] + ",";
    if (!params.empty()) ans += params[params.size() - 1];
    ans += ")";
    ans += body->toString();
    return ans;
}

sharedObj FuncDefExpression::accept(Context& context, Visitor& v) {
    return v.visit(context, this);
}

MemberAccessExpression::MemberAccessExpression(shared_ptr<Expression> obj_, const Token &name_, const vector<shared_ptr<Expression>>& args_)
    : obj(move(obj_)), name(name_.getValue()), args(args_) {}

string MemberAccessExpression::toString() {
    string res = name + "(";
    for (unsigned int i = 0; i < args.size() - 1; i++) {
        res += args[i]->toString() + ", ";
    }
    res += args[args.size() - 1]->toString();
    return res;
}

sharedObj MemberAccessExpression::accept(Context &context, Visitor &v) {
    return v.visit(context, this);
}
