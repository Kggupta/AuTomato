#include "Statement.h"
#include <utility>
#include "Result.h"

using namespace std;
typedef shared_ptr<statement> statementPtr;

bool flow::isType(type a) {
    return flow_type == a;
}

flow::flow(type t, sharedObj obj) : flow_type(t), value(obj) {};

flow::flow(type t) : flow_type(t) {}


flow simple_statement::execute(Context& context) {
    Visitor v;
    return flow(flow::type::SIMPLE, expr->accept(context, v));
};

simple_statement::simple_statement(shared_ptr<Expression> expr_) : expr(move(expr_)) {}

string simple_statement::toString() {
    return expr->toString();
}


while_statement::while_statement(shared_ptr<Expression> condition_, statementPtr statement_) 
	: condition(move(condition_)), statement(move(statement_)) {}

flow while_statement::execute(Context& context) {
    Visitor v;
    while (condition->accept(context, v)->value) {
        flow ret = statement->execute(context);
        if (ret.isType(flow::type::BREAK)) break;
        else if (ret.isType(flow::type::RET_T)) return ret;
    }
    return flow(flow::type::NONE);
};

string while_statement::toString() {
    string res = "while (";
    res += condition->toString();
    res += ")";
    res += statement->toString();
    return res;
}


for_statement::for_statement(
            Token identifier_, 
            shared_ptr<Expression> start_,
            shared_ptr<Expression> end_,
            shared_ptr<statement> toRun_,
            shared_ptr<Expression> step_
        ) : identifier(identifier_), start(start_), end(end_), toRun(toRun_), step(step_)
{}

flow for_statement::execute(Context& parent) {
    Visitor v;
    shared_ptr<SymbolTable> symbols = make_shared<SymbolTable>(parent.symbols);
    Context context(parent.name, symbols);
    sharedObj start_obj = start->accept(parent, v);
    sharedObj end_obj = end->accept(parent, v);
    if (!start_obj->value.isType<int>() || !end_obj->value.isType<int>()) {
        throw Error("TypeException", "Expected an integer type in for loop");
    }

    int inc = 1;

    if (step) {
        sharedObj step_obj = step->accept(context, v);
        if (!step_obj->value.isType<int>()) {
            throw Error("TypeException", "Expected an integer type in for loop iterator");
        }
        inc = step_obj->getValue<int>();
    }

    if (start_obj->getValue<int>() <= end_obj->getValue<int>()) {    
        for (int i = start_obj->getValue<int>(); i >= start_obj->getValue<int>() && i < end_obj->getValue<int>(); i += inc) {
            symbols->set(identifier.getValue(), make_shared<intType>(i));
            flow ret = toRun->execute(context);
            if (ret.isType(flow::type::BREAK)) break;
            else if (ret.isType(flow::type::RET_T)) return ret;
        }
    } else {    
        for (int i = start_obj->getValue<int>(); i <= start_obj->getValue<int>() && i > end_obj->getValue<int>(); i += inc) {
            symbols->set(identifier.getValue(), make_shared<intType>(i));
            flow ret = toRun->execute(context);
            if (ret.isType(flow::type::BREAK)) break;
            else if (ret.isType(flow::type::RET_T)) return ret;
        }
    }
    return flow(flow::type::NONE);
}

string for_statement::toString() {
    string res = "for (int i = ";
    res += start->toString();
    res += "; i < ";
    res += end->toString();
    res += ";";
    if (step) res += "i += " + step->toString();
    else res += "i++";
    res += ")";
    res += toRun->toString();
    return res;
}

block_statement::block_statement(vector<shared_ptr<statement>> statements) : statements(move(statements)) {}

flow block_statement::execute(Context& context) {
    for (auto & statement : statements) {
        flow ret = statement->execute(context);
        switch(ret.flow_type) {
            case flow::type::RET_T:
            case flow::type::BREAK:
            case flow::type::CONTINUE:
                return ret;
            default:
                continue;
        }
    }
    return flow(flow::type::NONE);
}

string block_statement::toString() {
    string res = "{\n";
    for (auto & statement : statements) {
        res += statement->toString() + "\n";
    }
    res += "}";
    return res;
}

if_statement::if_statement(
    shared_ptr<Expression> ifCondition, shared_ptr<statement> ifBlock, 
	vector<shared_ptr<Expression>> elifConditions, vector<shared_ptr<statement>> elifBlocks, 
	shared_ptr<statement> elseBlock) 
	: if_condition(move(ifCondition)), if_block(move(ifBlock)), elif_conditions(move(elifConditions)),
    elif_blocks(move(elifBlocks)), else_block(move(elseBlock)) {}

flow if_statement::execute(Context& context) {
    Visitor v;
    if (if_condition->accept(context, v)->value) {
        return if_block->execute(context);
    }
    for (unsigned int i = 0; i < elif_conditions.size(); i++) {
        if (elif_conditions[i]->accept(context, v)->value) {
            return elif_blocks[i]->execute(context);
        }
    }
    if (else_block) {
        return else_block->execute(context);
    }
    return flow(flow::type::NONE);
}

string if_statement::toString() {
    string res = "if (";
    res += if_condition->toString() + ")";
    res += if_block->toString();
    for (unsigned int i = 0; i < elif_conditions.size(); i++) {
        res += elif_conditions[i]->toString();
        res += elif_blocks[i]->toString();
    }
    if (else_block) {
        res += "else";
        res += else_block->toString();
    }
    return res;
}

return_statement::return_statement(shared_ptr<Expression> toReturn) : toReturn(toReturn) {}

flow return_statement::execute(Context& context) {
    Visitor v;
    if (toReturn) {
        return flow(flow::type::RET_T, toReturn->get()->accept(context, v));
    }
    else {
        return flow(flow::type::RET_T, make_shared<nullType>());
    }
}

string return_statement::toString() {
    return "return;";
}


flow break_statement::execute(Context& context) {
    return flow(flow::type::BREAK);
}

string break_statement::toString() {
    return "break;";
}


flow continue_statement::execute(Context& context) {
    return flow(flow::type::CONTINUE);
}

string continue_statement::toString() {
    return "continue;";
}
