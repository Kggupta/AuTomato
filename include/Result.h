#ifndef RESULT_H
#define RESULT_H

#include <memory>
#include <utility>
#include <variant>
#include <string>
#include <vector>

#include "Position.h"
#include "Error.h"
#include "types.h"

using namespace std;

class TypeException;
struct object;

class identifier {
    public:
        identifier() = default;
};

struct null {
    null() = default;
};

typedef variant<string, int, double, vector<shared_ptr<object>>, identifier, null> type;

struct Result {
    public:
        explicit Result(type a) : mResult(move(a)) {};

        type getResult() {return mResult;}

        string getStringType() {
            return visit([&](auto&& arg) {
                using T = decay_t<decltype(arg)>;
                return getStringType<T>();
            }, getResult());
        }

        template<typename T> 
        T getTypeOrThrow(const Position& pos) {
            if (auto i = get_if<T>(&mResult)) return *i;
            else {
                string r = "Expected type: '" + getStringType<T>() + "', but instead got '" + getStringType() + "'";

                throw runtime_error("TYPE EXCEPTION");
            }
        }

        template<typename T> 
        T& getValue() {
            return get<T>(mResult);
        }

        template<typename T>
        bool isType() {
            if (auto i = get_if<T>(&mResult)) return true;
            else return false;
        }

        operator int() const {
            return visit([&](auto&& arg) {
                using T = decay_t<decltype(arg)>;
                if constexpr(is_same_v<T, int>) return get<int>(mResult);
                else if constexpr(is_same_v<T, string>) {
                    if (!get<string>(mResult).empty()) {
                        return 1;
                    } else return 0;
                }
                else if (is_same_v<T, vector<shared_ptr<object>>>) {
                    if (!get<vector<shared_ptr<object>>>(mResult).empty()) return 1;
                    else return 0;
                }
                else if constexpr(is_same_v<T, double>) {
                    if (get<double>(mResult) != 0) return 1; else return 0;
                }
                else if constexpr(is_same_v<T, null>) {
                    return 0;
                }

                return 1;
            }, mResult);
        }

    private:
        type mResult;
        
        template<class T>
        string getStringType() {
            if constexpr(is_same_v<T, string>) return "string";
            else if constexpr(is_same_v<T, int>) return "integer";
            else if constexpr(is_same_v<T, double>) return "double";
            else if constexpr(is_same_v<T, identifier>) return "function";
            else if constexpr(is_same_v<T, shared_ptr<object>>) return "object";
            else if constexpr(is_same_v<T,vector<shared_ptr<object>>>) return "list";
            else if constexpr(is_same_v<T, null>) return "null";
            else return "unknown";
        }
};

#endif
