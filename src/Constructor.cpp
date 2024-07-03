#include "Constructor.h"


vector<pair<Production, int>> Constructor::getProductions(char lhs) const
{
    vector<pair<Production, int>> result;
    int index = 0;
    for (const auto& prod : productions)
    {
        if (prod.lhs == lhs)
        {
            result.push_back({ prod, index });
        }
        index++;
    }
    return result;
}

void Constructor::generateFirsts(){
    for (char nonterminal : nonterminals) {
        firsts[nonterminal] = set<char>();
        bool changed;
        do {
            changed = false;
            // 对每个产生式进行处理
            for (const Production& production : productions) {
                char lhs = production.lhs;
                const string& rhs = production.rhs;

                // 对产生式右部的每个符号进行处理
                for (char symbol : rhs) {
                    if (terminals.count(symbol)) {
                        // 如果是终结符，添加到 FIRST 集合中
                        if (!firsts[lhs].count(symbol)) {
                            firsts[lhs].insert(symbol);
                            changed = true;
                        }
                        break; // 一旦遇到终结符，就不再处理后续符号
                    }
                    else if (nonterminals.count(symbol)) {
                        // 如果是非终结符，将其 FIRST 集合添加到当前符号的 FIRST 集合中
                        size_t beforeSize = firsts[lhs].size();
                        firsts[lhs].insert(firsts[symbol].begin(), firsts[symbol].end());

                        if (firsts[lhs].size() != beforeSize) {
                            changed = true;
                        }

                        // 如果该非终结符的 FIRST 集合不包含 epsilon（假设用 '@' 表示），则不再处理后续符号
                        /*if (firsts[symbol].count(10) == 0) {
                            break;
                        }*/
                    }
                }
            }
        } while (changed);
    }
}
set<char> Constructor::first(char symbol)
{
    set<char> result;
    if (terminals.count(symbol))
    {
        result.insert(symbol);
    }
    else
    {
        result.insert(firsts[symbol].begin(), firsts[symbol].end());
    }
    return result;
}

set<Item> Constructor::closure(const set<Item>& items)
{
    set<Item> closureSet = items;
    queue<Item> workQueue;
    for (const auto& item : items)
    {
        workQueue.push(item);
    }

    while (!workQueue.empty())
    {
        Item item = workQueue.front();
        workQueue.pop();

        if (item.dotPos < item.prod.rhs.size())
        {
            char nextSymbol = item.prod.rhs[item.dotPos];
            if (nonterminals.count(nextSymbol))
            {
            
                    for (const auto& prod : getProductions(nextSymbol))
                    {
                    
                        if (item.dotPos + 1 < item.prod.rhs.size()) 
                        {
                            char expected = item.prod.rhs[item.dotPos + 1];
                            if (terminals.count(expected)) 
                            {
                                Item new_item(prod.first, prod.second, 0, expected);
                                if (!closureSet.count(new_item)) 
                                {
                                    closureSet.insert(new_item);
                                    workQueue.push(new_item);
                                }
                            }
                            else
                            {
                                for (auto ch : first(expected)) 
                                {
                                    Item new_item(prod.first, prod.second, 0, ch);
                                    if (!closureSet.count(new_item)) 
                                    {
                                        closureSet.insert(new_item);
                                        workQueue.push(new_item);
                                    }
                                }
                            }
                        }
                        else 
                        {
                            Item new_item(prod.first, prod.second, 0, item.lookahead);
                            if (!closureSet.count(new_item))
                            {
                                closureSet.insert(new_item);
                                workQueue.push(new_item);
                            }
                        }
                     }
                
            }
        }
    }
    return closureSet;
}

set<Item> Constructor::gotoSet(const set<Item>& items, char symbol)
{
    set<Item> gotoSet;
    for (const auto& item : items)
    {
        
        if (item.dotPos < item.prod.rhs.size() && item.prod.rhs[item.dotPos] == symbol)
        {
            gotoSet.insert(Item(item.prod, item.prod_num, item.dotPos + 1, item.lookahead));
        }
        if (item.dotPos == item.prod.rhs.size() && item.prod.rhs[item.dotPos] == symbol)
        {
            gotoSet.insert(Item(item.prod, item.prod_num, item.dotPos, item.lookahead));
        }
    }
    return gotoSet;
}

map<ItemSet, map<char, ItemSet>> Constructor::constructDFA()
{
    map<ItemSet, map<char, ItemSet>> dfa;

    ItemSet startSet;
    startSet.insert(Item(productions[0], 0 ,0, '#'));
    startSet.state = 0;
    int next_state_  = 0;
    
    //startSet.items = closure(startSet.items);

    queue<ItemSet> workQueue;
    startSet.items = closure(startSet.items);
    startSet.state = next_state_++;
    outFile << "State " << startSet.state << endl;
    for (auto& proc : startSet.items) {
        outFile << proc << endl;
    };
    workQueue.push(startSet);

    //workQueue.push(startSet);
    vector<ItemSet> visited;
    visited.push_back(startSet);

    while (!workQueue.empty())
    {
        ItemSet currentSet = workQueue.front();
        workQueue.pop();
        
        for (char symbol : terminals)
        {
            if (symbol == 0) continue;
            ItemSet newSet;
            newSet.items = gotoSet(currentSet.items, symbol);
            newSet.items = closure(newSet.items);
            ItemSet tmp = is_own(newSet, visited);
            if (!newSet.items.empty() && tmp.state == -1)
            {   
                newSet.state = next_state_++;
                visited.push_back(newSet);
                workQueue.push(newSet);
                dfa[currentSet][symbol] = newSet;
                outFile << "State " << newSet.state << " = Go(State " << currentSet.state<<", "<<reserve_list[symbol]<<")" << endl;
                for (auto& proc : newSet.items) {
                    outFile << proc << endl;
                };
            }
            else {
                if(!newSet.items.empty())
            {
                    outFile <<"  Go(State " << currentSet.state << ", " << reserve_list[symbol] << ") = State "<< tmp.state << endl;
                dfa[currentSet][symbol] = tmp;

            }
            }
        }

        for (char symbol : nonterminals)
        {
            ItemSet newSet;
            newSet.items = gotoSet(currentSet.items, symbol);
            ItemSet tmpSet = newSet;
            newSet.items = closure(newSet.items);
            
            ItemSet tmp = is_own(newSet, visited);
            if (!newSet.items.empty() && tmp.state == -1)
            {
                newSet.state = next_state_++;
                visited.push_back(newSet);
                workQueue.push(newSet);
                /*if (symbol == 'M') {
                    cout << "前：" << endl;
                    for (auto& item : tmpSet.items)
                    {
                        cout << item << endl;

                    }
                    cout << "后：" << endl;
                    for (auto& item : newSet.items)
                    {
                        cout << item << endl;

                    }
                }*/
                dfa[currentSet][symbol] = newSet;
                outFile << "State " << newSet.state << " = Go(State " << currentSet.state << ", " << reserve_list[symbol] << ")" << endl;
                for (auto& proc : newSet.items) {
                    outFile << proc << endl;
                };
            }
            else
            {
            if (!newSet.items.empty())
            {
                
                dfa[currentSet][symbol] = tmp;
            }

            }
        }
    }
    state_list = visited;
    return dfa;
}

pair<map<pair<int, char>, int>, map<pair<int, char>, Action>> Constructor::generateTables(const map<ItemSet, map<char, ItemSet>>& dfa)
{
    

    map<pair<int, char>, int> GOTO;
    map<pair<int, char>, Action> ACTION;
    vector<int> visited;
    for (int i = 0; i < state_list.size(); i++) {
        visited.push_back(0);
    }

    // 1、遍历dfa，吧所有的边对应的Gostate信息填入action表和goto表
    for (const auto& state : dfa)
    {
        /*for (auto& edge : state.second) {
            
        }*/
        visited[state.first.state] = 1;
        for (auto& item : state.first.items)
        {
            if (item.dotPos < item.prod.rhs.size())
            {
                char symbol = item.prod.rhs[item.dotPos];
                if (terminals.count(symbol))
                {
                   
                    if (!ACTION.count({ state.first.state, symbol }))
                    {
                    Action res{ SHIFT, state.second.find(symbol)->second.state };
                    ACTION[{state.first.state, symbol}] = res;
                        
                    }   
                    /*if (tmp.state != -1 && (res.type != tmp.type || res.state != tmp.state)) {
                        outFile << "冲突发生，State " << state.first.state << " -- " << reserve_list[symbol] << " -- > between State" << tmp.state << " and State " << res.state << endl;
                    }*/
                        
                }
                else 
                {
                    /*if(nonterminals.count(symbol))*/
                    /*cout <<  "State " << state.first.state << ", Symbol " << reserve_list[symbol] << " -> "  << endl;*/
                    GOTO[{ state.first.state, symbol}] = { state.second.find(symbol)->second.state };
                   
                }
            }
            else 
            {
                if (item.prod.lhs == startSymbol && item.lookahead == '#')
                {
                    ACTION[{state.first.state, item.lookahead}] = { ACCEPT, 0 };
                }
                else
                {
                   
                    ACTION[{state.first.state, item.lookahead}] = { REDUCE, item.prod_num };
                }
            }
            
        }
    }

    //2、 此时剩余的没有被访问到的状态就是只含有规约项的状态
    for (int i = 0; i < visited.size(); i++) {
        if (visited[i]) continue;
        for (auto& item : state_list[i].items)
        {
            /*if (item.dotPos != item.prod.rhs.size()) cout << "不是规约项，你想错了！" << endl;
            if (GOTO.count({ state_list[i].state, item.lookahead })) cout << "规约项有冲突" << endl;*/
            if (item.prod.lhs == startSymbol && item.lookahead == '#')
            {

                cout << state_list[i].state << endl;
                ACTION[{state_list[i].state, item.lookahead}] = {ACCEPT, 0 };

            }
            else 
            {
                ACTION[{state_list[i].state, item.lookahead}] = {REDUCE ,item.prod_num };

            }
        }
    }

    outFile << "Action Table:" << endl;
    for (const auto& entry : ACTION) {
        outFile << "State " << entry.first.first << ", Symbol " << reserve_list[entry.first.second] << " -> (" << entry.second.type << ", "<< entry.second.state <<" )" << endl;
        
    }

    outFile << "Goto Table:" << endl;
    for (const auto& entry : GOTO)
    {
        outFile << "State " << entry.first.first << ", Symbol " << reserve_list[entry.first.second] << " -> " << entry.second<< endl;
    }
    outFile.close();
    return { GOTO, ACTION };
}