#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <variant>
#include <vector>

#include "Context.h"
#include "Statement.h"
#include "Interpreter.h"
#include "Token.h"
#include "types.h"

using namespace std;

class Visitor;
struct Result;
struct Context;
class statement;

class Expression {
    public:
        Expression();
        virtual string toString();
        virtual sharedObj accept(Context& context, Visitor& v);
        virtual Token getToken();
        virtual ~Expression();
    protected:
        Token tok;
};

class PostfixExpression : public Expression {
    public:
        PostfixExpression(shared_ptr<Expression> left, Token tok);
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        shared_ptr<Expression> left;
};

class PrefixExpression : public Expression {
    public:
        PrefixExpression(Token tok, shared_ptr<Expression> right);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        shared_ptr<Expression> right;
};

class NameExpression : public Expression {
    public:
        NameExpression(string name, Token tok);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        string name;
};

class NumberExpression : public Expression {
    public:
        NumberExpression(const Token& t);
        string toString() override;
        int getInt();
        double getDouble();
        sharedObj accept(Context& context, Visitor& v) override;
    private:
        variant<int, double> value;
};

class StringExpression : public Expression {
    public:
        StringExpression(const Token& t);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        string getValue();
    private:
        string value;
};

class BinOpExpression : public Expression {
    public:
        BinOpExpression(shared_ptr<Expression> left, Token op, shared_ptr<Expression> right);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        shared_ptr<Expression> left;
        shared_ptr<Expression> right;
};

class ComparisonExpression : public Expression {
    public:
        ComparisonExpression(shared_ptr<Expression> left, Token op, shared_ptr<Expression> right);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        shared_ptr<Expression> left;
        shared_ptr<Expression> right;
};

class AssignmentExpression : public Expression {
    public:
        AssignmentExpression(string name, shared_ptr<Expression> right, Token eq);
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        string name;
        shared_ptr<Expression> right;
};

class UpdateExpression : public Expression {
    public:
        UpdateExpression(string name, shared_ptr<Expression> right, Token eq);
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        string name;
        shared_ptr<Expression> right;
};

class ObjectIndexUpdateExpression : public Expression {
    public:
        ObjectIndexUpdateExpression(
            const Token& tok, 
            shared_ptr<Expression> objToUpdate,
            shared_ptr<Expression> index, 
            shared_ptr<Expression> newVal);
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        Token tok;
        shared_ptr<Expression> objToUpdate;
        shared_ptr<Expression> index;
        shared_ptr<Expression> newVal;
};

class ConditionalExpression : public Expression {
    public:
        ConditionalExpression(
            Token tok_, 
            shared_ptr<Expression> condition, 
            shared_ptr<Expression> then, 
            vector<shared_ptr<Expression>> elif_conditions, 
            vector<shared_ptr<Expression>> elif_thens, 
            shared_ptr<Expression> elseBranch);
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        shared_ptr<Expression> condition;
        shared_ptr<Expression> thenBranch;
        vector<shared_ptr<Expression>> elif_conditions; 
        vector<shared_ptr<Expression>> elif_thens;
        shared_ptr<Expression> elseBranch;
};

class CallFunctionExpression : public Expression {
    public:
        CallFunctionExpression(
            shared_ptr<Expression> left,
            Token tok_, 
            vector<shared_ptr<Expression>> params
        );
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        shared_ptr<Expression> left;
    vector<shared_ptr<Expression>> params;
};

class IndexExpression : public Expression {
    public:
        IndexExpression(
            shared_ptr<Expression> left,
            Token tok_, 
            shared_ptr<Expression> index
        );
        sharedObj accept(Context& context, Visitor& v) override;
        string toString() override;
        shared_ptr<Expression> index;
        shared_ptr<Expression> left;
};

class ListExpression : public Expression {
    public:
        ListExpression(Token t, vector<shared_ptr<Expression>> values);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        vector<shared_ptr<Expression>> getValue();
    private:
        vector<shared_ptr<Expression>> value;
};

class FuncDefExpression : public Expression {
    public:
        FuncDefExpression(Token t, string name, vector<string> params, shared_ptr<statement> toRun, bool anonymous);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        vector<string> params;
        string name;
        shared_ptr<statement> body;
        bool lambda = false;
};

class MemberAccessExpression : public Expression {
    public:
        MemberAccessExpression(shared_ptr<Expression> obj, const Token& name, const vector<shared_ptr<Expression>>& args);
        string toString() override;
        sharedObj accept(Context& context, Visitor& v) override;
        shared_ptr<Expression> obj;
        const string name;
        const vector<shared_ptr<Expression>> args;
};

#endif
