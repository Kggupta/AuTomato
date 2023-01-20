#ifndef STATEMENT_H
#define STATEMENT_H

#include "Context.h"
#include "Expression.h"
#include "Token.h"
#include "Result.h"
#include "types.h"

#include <vector>
#include <memory>
#include <optional>

using namespace std;

class Expression;
struct Context;

class flow {
    public:
        enum class type {
            SIMPLE,
            RET_T,
            CONTINUE,
            BREAK,
            NONE
        };
        type flow_type;
        optional<shared_ptr<object>> value;
        bool isType(type a);
        flow(type, shared_ptr<object> obj);
        flow(type);
};

class statement {
	public:
		statement() = default;
		virtual flow execute(Context& context) {
            return flow(flow::type::NONE);
        };
        virtual string toString() {return "";};
		virtual ~statement() = default;
};

class simple_statement : public statement {
    public:
        flow execute(Context& context) override;
        string toString() override;
        simple_statement(shared_ptr<Expression> expr);
    private:
        shared_ptr<Expression> expr;
};

class if_statement : public statement {
    public:
        if_statement(
            shared_ptr<Expression> ifCondition, 
            shared_ptr<statement> ifBlock, 
            vector<shared_ptr<Expression>> elifConditions, 
            vector<shared_ptr<statement>> elifBlocks, 
            shared_ptr<statement> elseBlock);

        flow execute(Context& context) override;
        string toString() override;
    private:
        shared_ptr<Expression> if_condition;
        shared_ptr<statement> if_block;
        vector<shared_ptr<Expression>> elif_conditions;
        vector<shared_ptr<statement>> elif_blocks;
        shared_ptr<statement> else_block;
};

class while_statement : public statement {
    public:
        while_statement(
            shared_ptr<Expression> condition_,
            shared_ptr<statement> statement_
        );
        flow execute(Context& context) override;
        string toString() override;
    private:
        shared_ptr<Expression> condition;
        shared_ptr<statement> statement; 
};

class for_statement : public statement {
    public:
        for_statement(
            Token identifier, 
            shared_ptr<Expression> start,
            shared_ptr<Expression> end,
            shared_ptr<statement> toRun,
            shared_ptr<Expression> step = nullptr
        );
        string toString() override;
        flow execute(Context& context) override;
    private:
        Token identifier; 
        shared_ptr<Expression> start;
        shared_ptr<Expression> end;
        shared_ptr<Expression> step;
        shared_ptr<statement> toRun;
        
};

class block_statement : public statement {
    public:
        block_statement(vector<shared_ptr<statement>> statements);
        flow execute(Context& context) override;
        string toString() override;
    private:
        vector<shared_ptr<statement>> statements;
};

class return_statement : public statement {
    public:
        return_statement(shared_ptr<Expression> toReturn);
        return_statement() = default;
        flow execute(Context& context) override;
        string toString() override;
    private:
        optional<shared_ptr<Expression>> toReturn;
};

class break_statement : public statement {
    public:
        break_statement() = default;
        flow execute(Context& context) override;
        string toString() override;

};

class continue_statement : public statement {
    public:
            continue_statement() = default;
            flow execute(Context& context) override;
            string toString() override;
};

#endif
