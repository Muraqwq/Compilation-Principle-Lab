#pragma once

#include "string"
#include "vector"
#include "Reserve.h"

using namespace std;
enum E_TYPE
{
    TEMP,
    RESERVE
};
class Tmp
{
public:
    int val;
    int index;
    string var{};
};

class Symbol
{
public:
    vector<Symbol*> child_nodes;
};

class E : public Symbol
{
public:
    vector<int> truelist{};
    vector<int> falselist{};
    E_TYPE type;
    Tmp place{};
    bool is_num() {
        return place.index == -1;
    }
    string print() {
        
            if (place.index == -2) return place.var;
            else return to_string(place.val);
        

       
    }
};

class S : public Symbol
{
public:
    vector<int> nextlist{};
};

class L : public Symbol
{
public:
    vector<int> nextlist{};
};

class A : public Symbol
{
};

class N : public Symbol
{
public:
    vector<int> nextlist{};
};

class M : public Symbol
{
public:
    int quad;
};

class I : public Symbol
{
public:
    string val;
    int place;
    VAR_TYPE type;
   
};

/**定义relop操作*/
class R : public Symbol
{
public:
    char key;
    string val;
};

/** 定义计算操作*/
class C : public Symbol
{
public:
    char key;
};

class K : public Symbol
{
public:
    char key;
};

class B : public Symbol
{
public:
    char key;
};