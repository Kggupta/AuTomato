#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <math.h>
#include <memory>
#include <variant>

#include "helper.h"
#include "Expression.h"
#include "Result.h"
#include "types.h"

typedef std::shared_ptr<object> sharedObj;
 
struct Context;

class Expression;
class PrefixExpression;
class PostfixExpression;
class BinOpExpression;
class ComparisonExpression;
class NumberExpression;
class StringExpression;
class ListExpression;
class AssignmentExpression;
class UpdateExpression;
class ObjectIndexUpdateExpression;
class NameExpression;
class ConditionalExpression;
class CallFunctionExpression;
class IndexExpression;
class FuncDefExpression;
class MemberAccessExpression;

class Visitor {
    public:
        sharedObj visit(Context& context, Expression* e);
        sharedObj visit(Context& context, PrefixExpression* e);
        sharedObj visit(Context& context, PostfixExpression* e);
        sharedObj visit(Context& context, BinOpExpression* e);
        sharedObj visit(Context& context, ComparisonExpression* e);
        sharedObj visit(Context& context, NumberExpression* e);
        sharedObj visit(Context& context, StringExpression* e);
        sharedObj visit(Context& context, ListExpression* e);
        sharedObj visit(Context& context, AssignmentExpression* e);
        sharedObj visit(Context& context, UpdateExpression* e);
        sharedObj visit(Context& context, ObjectIndexUpdateExpression* e);
        sharedObj visit(Context& context, NameExpression* e);
        sharedObj visit(Context& context, ConditionalExpression* e);
        sharedObj visit(Context& context, CallFunctionExpression* e);
        sharedObj visit(Context& context, IndexExpression* e);
        sharedObj visit(Context& context, FuncDefExpression* e);
        sharedObj visit(Context& context, MemberAccessExpression* e);
};

#endif
