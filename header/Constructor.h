#pragma once

#include "fstream"
#include "Grammer.h"
#include "iostream"
#include "map"
#include "queue"
#include "Reserve.h"
#include "set"
#include "stdlib.h"

using namespace std;

enum ActionType
{
    SHIFT,
    REDUCE,
    ACCEPT,
    ERROR,
    MREDUCE,
    NREDUCE
};

struct Action
{
    ActionType type;
    int state; // 对于SHIFT操作，state表示下一个状态；对于REDUCE操作，state表示产生式的编号
};


class Item
{
public:
    Item(const Production& prod, int prod_num, int dotPos, char lookahead) : prod(prod), prod_num(prod_num),dotPos(dotPos), lookahead(lookahead) {};

    bool operator==(const Item& other) const
    {
        return prod.lhs == other.prod.lhs && prod.rhs == other.prod.rhs && dotPos == other.dotPos && lookahead == other.lookahead;
    }

    

    bool operator<(const Item& other) const
    {
        if (prod.lhs != other.prod.lhs)
            return prod.lhs < other.prod.lhs;
        if (prod.rhs != other.prod.rhs)
            return prod.rhs < other.prod.rhs;
        if (dotPos != other.dotPos)
            return dotPos < other.dotPos;
        return lookahead < other.lookahead;
    }

    friend ostream& operator <<(ostream& os, const Item& item){
        os << "[ " << item.prod.lhs << " -> ";
        for (int i = 0; i < item.prod.rhs.size(); i++) {
            if (item.dotPos == i) os << ". ";
            os << reserve_list[item.prod.rhs[i]] << " ";
        }
        if(item.dotPos == item.prod.rhs.size()) os << ". ";
        os <<", " << reserve_list[item.lookahead] << " ]";
        return os;
    }
    

    Production prod;
    int prod_num;
    int dotPos;
    char lookahead;
};

class ItemSet
{
public:
    set<Item> items{};
    int state{};
    void insert(const Item& item) { items.insert(item); }

    bool operator==(const ItemSet& other) const
    {
        return items == other.items;
    }
    
    bool operator<(const ItemSet& other) const
    {
        return items < other.items;
    }

};

class Constructor
{
public:
    Constructor(const vector<Production>& productions, char startSymbol) : productions(productions), startSymbol(startSymbol)
    {
   
        generateFirsts();
    };

    friend ostream& operator <<(ostream& os, const Production& prod) {
        os << prod.lhs << " -> ";
        for (int i = 0; i < prod.rhs.size(); i++) {
            
            os << reserve_list[prod.rhs[i]] << " ";
           
        }
        return os;
    }
    
    vector<pair<Production, int>> getProductions(char lhs) const;
    void generateFirsts();
    ItemSet is_own(const ItemSet& tar, const vector<ItemSet>& visited) {
        
        for (auto& items : visited) {
            if (includes(tar.items.begin(), tar.items.end(), items.items.begin(), items.items.end()))
                return items;
        }
        ItemSet tmp{};
        tmp.state = -1;
        return tmp;
    }
    set<char> first(char symbol);

    set<Item> closure(const set<Item>& items);
    set<Item> gotoSet(const set<Item>& items, char symbol);
    map<ItemSet, map<char, ItemSet>> constructDFA();
   pair<map<pair<int, char>, int>, map<pair<int, char>, Action>> generateTables(const map<ItemSet, map<char, ItemSet>>& dfa);
    
private:
    vector<Production> productions{};
    map<char, set<char>> firsts{};
    char startSymbol{};
    vector<ItemSet> state_list{};
    fstream outFile{ "LR_Generate.txt", std::ios::out | std::ios::trunc };
};

