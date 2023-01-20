#include "Interpreter.h"
#include "optional"
#include "Statement.h"
#include "Error.h"
#include "Token.h"

using namespace std;

sharedObj Visitor::visit(Context& context, Expression* e) {
    throw UndefinedOperationException(e->getToken().startPos, "Visited unknown expression " + e->toString());
}

sharedObj Visitor::visit(Context& context, PrefixExpression* e) {
    if (e->getToken().isOneOf(Token::Type::INC, Token::Type::DEC)) {
        string name = e->right->getToken().getValue();
        optional<sharedObj> value = context.symbols->get(name);
        if (!value) throw UndefinedVariable(make_shared<Context>(context), e->right->getToken().getValue(), e->getToken().startPos);
        sharedObj val = value->get()->inc();
        if (!context.symbols->update(name, val)) throw UndefinedVariable(make_shared<Context>(context), e->right->getToken().getValue(), e->right->getToken().startPos);
        return val;
    }
    sharedObj rightSide = e->right->accept(context, *this);
    switch(e->getToken().type) {
        case Token::Type::PLUS:
            return rightSide->prePlus();
        case Token::Type::MINUS: {
            return rightSide->preMinus();
        }
        case Token::Type::NOT: {
            return rightSide->preNot();
        }
        case Token::Type::BNOT:
            return rightSide->bnot();
        default:
            throw UndefinedOperationException(e->getToken().startPos, "Visited unknown unary expression: " + e->getToken().getValue());
    }
    return rightSide;
}

sharedObj Visitor::visit(Context& context, PostfixExpression* e) {
    if (e->getToken().isOneOf(Token::Type::INC, Token::Type::DEC)) {
        string name = e->left->getToken().getValue();
        optional<sharedObj> value = context.symbols->get(name);
        if (!value) throw UndefinedVariable(make_shared<Context>(context), e->left->getToken().getValue(), e->left->getToken().startPos);
        sharedObj val = nullptr;
        if (e->getToken().is(Token::Type::INC)) val = value->get()->inc();
        else if (e->getToken().is(Token::Type::DEC)) val = value->get()->dec();
        else throw UndefinedOperationException(e->getToken().startPos, e->getToken().getValue(), value->get()->value.getStringType());

        bool success = context.symbols->update(name, val);
        if (!success) throw UndefinedVariable(make_shared<Context>(context), e->left->getToken().getValue(), e->left->getToken().startPos);
        return *value;
    }
    throw UndefinedOperationException(e->getToken().startPos, "Visited unknown unary operation: " + e->getToken().getValue());
}

sharedObj Visitor::visit(Context& context, BinOpExpression* e) {
    sharedObj left = e->left->accept(context, *this);
    sharedObj right = e->right->accept(context, *this);
    switch(e->getToken().type) {
        case Token::Type::PLUS:
            return left->addBy(right);
        case Token::Type::MINUS:
            return left->subBy(right);
        case Token::Type::MULT:
            return left->multBy(right);
        case Token::Type::DIV:
            return left->divBy(right);
        case Token::Type::MOD:
            return left->mod(right);
        case Token::Type::POWER:
            return left->powBy(right);
        case Token::Type::BAND:
            return left->band(right);
        case Token::Type::BOR:
            return left->bor(right);
        case Token::Type::XOR:
            return left->bxor(right);
        case Token::Type::LSHIFT:
            return left->lshift(right);
        case Token::Type::RSHIFT:
            return left->rshift(right);
        default:
            throw UndefinedOperationException(e->getToken().startPos, e->getToken().getValue());
    }
}

sharedObj Visitor::visit(Context& context, ComparisonExpression* e) {
    sharedObj left = e->left->accept(context, *this);
    sharedObj right = e->right->accept(context, *this);
    switch(e->getToken().type) {
        case Token::Type::NE:
            return left->ne(right);
        case Token::Type::EE:
            return left->ee(right);
        case Token::Type::GE:
            return left->gte(right);
        case Token::Type::LE:
            return left->lte(right);
        case Token::Type::GT:
            return left->gt(right);
        case Token::Type::LT:
            return left->lt(right);
        case Token::Type::AND:
            return make_shared<intType>(left->toBool()->value && right->toBool()->value);
        case Token::Type::OR:
            return make_shared<intType>(left->toBool()->value || right->toBool()->value);
        default:
            throw UndefinedOperationException(e->getToken().startPos, e->getToken().getValue());
    }
}

sharedObj Visitor::visit(Context& context, NumberExpression* e) {
    if (e->getToken().is(Token::Type::INT)) return make_shared<intType>(e->getInt());
    else if (e->getToken().is(Token::Type::DOUBLE)) return make_shared<doubleType>(e->getDouble());
    throw Error(e->getToken().startPos, "Unknown Error: Not integer or double type" + e->getToken().getValue());
}

sharedObj Visitor::visit(Context& context, StringExpression* e) {
    return make_shared<stringType>(e->getValue());
}
sharedObj Visitor::visit(Context& context, ListExpression* e) {
    vector<sharedObj> items;
    for (unsigned int i = 0; i < e->getValue().size(); i++) {
        items.push_back(e->getValue()[i]->accept(context, *this));
    }
    return make_shared<listType>(items);
}

sharedObj Visitor::visit(Context& context, AssignmentExpression* e) {
    sharedObj res = e->right->accept(context, *this);
    context.symbols->set(e->name, res);
    return res;
}
sharedObj Visitor::visit(Context& context, UpdateExpression* e) {
    sharedObj res = e->right->accept(context, *this);
    if (e->getToken().type == Token::Type::EQUALS) {
        if (context.symbols->update(e->name, res)) return res;
        else throw UndefinedVariable(make_shared<Context>(context), e->name, e->getToken().startPos);
    }
    optional<sharedObj> cur = context.symbols->get(e->name);
    if (!cur) throw UndefinedVariable(make_shared<Context>(context), e->name, e->getToken().startPos);
    switch(e->getToken().type) {
        case Token::Type::PLUS_EQUAL:
            cur = cur->get()->addBy(res);
            break;
        case Token::Type::MINUS_EQUAL:
            cur = cur->get()->subBy(res);
            break;
        case Token::Type::MULT_EQUAL:
            cur = cur->get()->multBy(res);
            break;
        case Token::Type::DIV_EQUAL:
            cur = cur->get()->divBy(res);
            break;
        case Token::Type::MOD_EQUAL:
            cur = cur->get()->mod(res);
            break;
        default:
            throw UndefinedOperationException(e->getToken().startPos, "Undefined Operation", e->getToken().getValue());
    }
    context.symbols->update(e->name, *cur);
    return make_shared<nullType>();
}

sharedObj Visitor::visit(Context& context, ObjectIndexUpdateExpression* e) {
    sharedObj res = e->newVal->accept(context, *this);
    sharedObj obj = e->objToUpdate->accept(context, *this);
    sharedObj index = e->index->accept(context, *this);
    sharedObj newValue = e->newVal->accept(context, *this);
    
    return obj->indexUpdate(index, newValue);
}


sharedObj Visitor::visit(Context& context, NameExpression* e) {
    optional<sharedObj> value = context.symbols->get(e->name);
    if (value) {
        return *value;
    }
    throw UndefinedVariable(make_shared<Context>(context), e->name, e->getToken().startPos);
}

sharedObj Visitor::visit(Context& context, ConditionalExpression* e) {
    if (e->condition->accept(context, *this)->value) {
        return e->thenBranch->accept(context, *this);
    } 
    for (unsigned int i = 0; i< e->elif_conditions.size(); i++) {
        if (e->elif_conditions[i]->accept(context, *this)->value) {
            return e->elif_thens[i]->accept(context, *this);
        }
    }
    return e->elseBranch->accept(context, *this);
}

sharedObj Visitor::visit(Context& context, CallFunctionExpression* e) {
    sharedObj decl = e->left->accept(context, *this);
    vector<sharedObj> args;
    for (auto & param : e->params) {
        args.push_back(param->accept(context, *this));
    }
    return decl->call(context, args);
}

sharedObj Visitor::visit(Context& context, IndexExpression* e) {
    sharedObj index_obj = e->index->accept(context, *this);
    return e->left->accept(context, *this)->index(index_obj);
}

sharedObj Visitor::visit(Context& context, FuncDefExpression* e) {
    shared_ptr<functionType> fun = make_shared<functionType>(e->name, e->params, e->body); 
    if (!e->lambda) context.symbols->set(e->name, fun);
    return fun;
}

sharedObj Visitor::visit(Context &context, MemberAccessExpression *e) {
    sharedObj left = e->obj->accept(context, *this);
    vector<sharedObj> args;
    for (auto & arg : e->args) args.push_back(arg->accept(context, *this));
    return left->dot(e->name, args);
}
