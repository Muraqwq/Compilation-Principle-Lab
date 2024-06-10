#pragma once
#include "map"
#include "set"
#include "string"

enum Token
{
    PLUS = 13,   // +
    TIMES,       // -
    LEFT_PAREN,  // (
    RIGHT_PAREN, // )
    EQUAL,       // =
    GREATER,     // >
    LESS,        // <
    SEMICOLON,   // ;
    COMMA,       // ,
    COLON,       // :
    ASSIGN,      // :=
    AND,         // &&
    OR,          // ||
    NOT,       // !
    LEFT_CURLY_BRACE,
    RIGHT_CURLY_BRACE

};

enum VAR_TYPE
{
    VAR = 11,
    INT
};




using namespace std;
extern map<char, string> reserve_list;
extern set<char> terminals;
extern set<char> nonterminals;
extern map<string, int> var_lists;
