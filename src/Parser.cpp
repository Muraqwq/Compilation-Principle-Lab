#include "Parser.h"

Parser::Parser() : lex_parser("./input.txt"), translate{}, methods{ translate }, grammers{ methods }, constructor{grammers.grammer_list, 'V'}{
    
    auto lr_res = constructor.generateTables(constructor.constructDFA());
        GOTO = lr_res.first;
    ACTION = lr_res.second;
};
void Parser::prework()
{
    lex_parser.run();
    /*var_lists = lex_parser.generate_var_lists();*/
    input = lex_parser.get_res();
    vector<pair<char, string>> tmp;
    for (auto& symbol : input)
    {
        if ((symbol.first == 2 || symbol.first == 28 )and tmp[tmp.size() - 1].first == 10) tmp.pop_back();
        if (symbol.first == 7) 
            tmp.push_back({ 10, "-" });
        
        tmp.push_back(symbol);
        if ((symbol.first <= 9 and symbol.first >= 6) or symbol.first == 20 )
            tmp.push_back({ 10, "-"});
            
    }
    input = tmp;
}

void Parser::error(int pos, char ch, string msg)
{
    cerr << "语法分析出现错误！ 位置：" << to_string(pos) << ", 符号标识：" << reserve_list[ch] << ", 原因：" << msg << endl;
    return;
}

void Parser::run()
{
    input.push_back({ '#', ""});
    stack_symbol.push({ nullptr, '#', "" });
    stack_state.push(0);
    while (index < input.size())
    {
        //cout << "当前下标：" << index << endl;
        int cur_state = stack_state.top();
        Symbol_Item cur_symbol;
        if (!isReserve(input[index].first))
        {
            if (needWrap(input[index].first))
            {
                cur_symbol = { TransMethod::Item2Symbol(input[index].first, input[index].second), input[index].first, input[index].second };
            
                if (cur_symbol.key == 11 || cur_symbol.key == 12) cur_symbol.key = 'I';
                else if (cur_symbol.key <= 19 and cur_symbol.key >= 17) cur_symbol.key = 'R';
                else if (cur_symbol.key == 13 || cur_symbol.key == 14) cur_symbol.key = 'C';
                else if (cur_symbol.key == 24 || cur_symbol.key == 25) cur_symbol.key = 'B';
                else {}
            }
        }
        else
        {
            cur_symbol = { nullptr, input[index].first, input[index].second };
        }

        auto actionIt = ACTION.find({ cur_state, cur_symbol.key });
        if (actionIt == ACTION.end())
        {
            error(index, cur_symbol.key, "无法匹配到动作！");
            return;
        }

        Action action = actionIt->second;
        switch (action.type)
        {
        case SHIFT:
            stack_state.push(action.state);
            stack_symbol.push(cur_symbol);
            index++;
            break;
        case REDUCE:
        {
            cout << "规约了，使用产生式 " << action.state << endl;
            const Production& production = grammers.getProduction(action.state);
            deque<Symbol*> temp_symbol_queue{};
            for (int i = 0; i < production.rhs.size(); i++)
            {
                if (stack_symbol.top().symbol != nullptr)
                    temp_symbol_queue.push_front(stack_symbol.top().symbol);
                stack_state.pop();
                stack_symbol.pop();
            }

            stack_state.push(GOTO[{stack_state.top(), production.lhs}]);
            Symbol* tmp = TransMethod::Item2Symbol(production.lhs, "");
            for (auto* item : temp_symbol_queue)
            {
                tmp->child_nodes.push_back(item);
            }
            stack_symbol.push({ tmp, production.lhs, "" });
            try {
                production.method(tmp);
            
            }
            catch (runtime_error& e)
            {
                error(index, cur_symbol.key, e.what());
            }
        }
        break;

        case ACCEPT:
            cout << "程序翻译成功！" << endl;
            return;
            break;
        case ERROR:
            error(index, cur_symbol.key, "匹配表中未定义该项！");
            return;
            break;
        case MREDUCE:
            const Production& prod = grammers.getProduction(15);
            M* tmp = new M();
            prod.method(tmp);
            
            break;
        }

    }
}