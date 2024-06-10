#pragma once

#include "vector"
#include "functional"
#include "string"
#include "Methods.h"
using namespace std;

 

class Production
{
public:
    Production(char lhs, string rhs, function<void(Symbol*)> method) : lhs(lhs), rhs(rhs), method(method) {};
    Production(char lhs, string rhs, function<void(Symbol*)> method, string pattern) : lhs(lhs), rhs(rhs), method(method), pattern(pattern) {};

    char lhs;                        // 左部
    string rhs;                      // 右部
    function<void(Symbol*)> method; // 对应动作
    string pattern{};
    friend ostream& operator <<(ostream& os, const Production& prod);
    template <typename Func, typename... Args>
    void setFunction(A& obj, Func&& method, Args &&...args)
    {
        method = std::bind(std::forward<Func>(method), &obj, std::forward<Args>(args)...);
    }

    void callFunction(Symbol* s)
    {
        if (method)
        {
            method(s);
        }
    }
};

class Grammer
{
public:
    Grammer(TransMethod& method);
    const Production& getProduction(int target);
    vector<Production> grammer_list;

private:
    TransMethod& methods;
};

