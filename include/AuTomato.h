#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <iostream>

#include "Lexer.h"
#include "Error.h"
#include "Parser.h"
#include "Expression.h"
#include "Token.h"
#include "Context.h"

using namespace std;

namespace automato {
    int run(const char * command, Context& context, bool shell = false, string fileName = "automaton") {
        Lexer lexer(fileName, command);
        vector<Token> a;
        try {
            a = lexer.getTokens();
        } catch (exception& error) {
            cout << error.what() << endl;
            return 1;
        }

        Parser parser(a);
        shared_ptr<statement> statement;

        while (parser.lookAhead(0).isNot(Token::Type::END)) {
            try {
                statement = parser.parse();

                if (!statement) continue;
            } catch (exception& error) {
                cout << error.what() << endl;
                return 1;
            }

            if (!statement) continue;

            try {
                flow res = statement->execute(context);
                switch (res.flow_type) {
                    case flow::type::RET_T:
                        throw SyntaxError("No function corresponding to return.");
                        break;
                    case flow::type::BREAK:
                        throw SyntaxError("No loop corresponding to break.");
                        break;
                    case flow::type::CONTINUE:
                        throw SyntaxError("No loop corresponding to continue.");
                        break;
                    default:
                        if (shell) cout << res.value->get()->toString() << endl;
                        break;
                }
            } catch (exception& error) {
                cout << error.what() << endl;
                return 1;
            }
        }
        return 0;
    }
}

#endif
