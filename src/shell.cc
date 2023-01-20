#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <readline/readline.h>
#include <readline/history.h>
#include "AuTomato.h"
#include "stdfuncs.h"

using namespace std;

typedef function<optional<variant<string, int, double, vector<sharedObj>, identifier, null>>(vector<sharedObj>)> natFun;
int main(int arg_count, char * arg_list[]) {
    string toRun;
    string command;
    shared_ptr<SymbolTable> symbols = make_shared<SymbolTable>();
    symbols->set("false", Result(0));
    symbols->set("true", Result(1));
    Context global("<automaton>", symbols);
    addStdFunctions(global);
    if (arg_count == 1) {
        while (1) {
            char * line = readline("automato>> ");
            if (!line) break;
            if (*line) add_history(line);
            if (*line == *"exit") break;
            automato::run(line, global, true);
            free(line);
        }
    }
    else if (arg_count == 2) {
        ifstream myfile (arg_list[1]);
        if (myfile.is_open())
        {
            while ( getline (myfile,command) )
            {
                toRun += command +  '\n';
            }
            myfile.close();
        } else {
            cout << "Could not open file " << arg_list[1] << endl;
            return 1;
        }
        automato::run(toRun.c_str(), global, false, arg_list[1]);
    }
    return 0;
}