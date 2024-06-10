#pragma once

#include "iostream"
#include "memory"
#include "stack"
#include "stdlib.h"

#include "Constructor.h"
#include "Lex.h"
#include "Methods.h"
#include "Symbol.h"
#include "Translate.h"


struct Symbol_Item
{
    Symbol* symbol;
    char key;
    string val;
};



class Parser
{
public:
    Parser();
    void run();
    void error(int pos, char ch, string msg);
    void prework();
    inline void generate_intermediate_code() {
        translate.print_res(intermediate_code_file);
    }
    friend ostream& operator << (ostream& os, stack<Symbol_Item> sym_stack) {
        os << "#Õ»µ×# ";
        return os;
    }
private:
    Lex lex_parser;
    Translate translate;
    TransMethod methods;
    Grammer grammers;
    Constructor constructor;

private:
    std::map<std::pair<int, char>, int> GOTO;
    std::map<std::pair<int, char>, Action> ACTION;

    stack<int> stack_state{};
    stack<Symbol_Item> stack_symbol{};
    

private:
    fstream intermediate_code_file{ "./QUATERNARY_LIST.txt", std::ios::out | std::ios::trunc };
    vector<pair<char, string>> input;
    int index{};
    void parse();
    inline bool isReserve(char ch) { return terminals.count(ch); }
    inline bool needWrap(char ch) { return !(ch == 15 || ch == 16 || (ch >= 20 and ch <= 23) || ch >= 26); }
};


// Parser::Parser()
// {

// }
