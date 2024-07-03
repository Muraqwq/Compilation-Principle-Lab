#include "iostream"
#include "string"
#include "vector"
#include "fstream"
#include "sstream"
#include "set"
#include "map"
#include "Reserve.h"
using namespace std;


class Lex
{

public:
    Lex() {};
    Lex(string path) : file_path(path)
    {
        reserve_list.insert({ {"program", 0} });
        reserve_list.insert({ {"begin", 1} });
        reserve_list.insert({ {"end", 2} });
        reserve_list.insert({ {"var", 3} });
        reserve_list.insert({ {"integer", 4} });
        reserve_list.insert({ {"if", 5} });
        reserve_list.insert({ {"then", 6} });
        reserve_list.insert({ {"else", 7} });
        reserve_list.insert({ {"do", 8} });
        reserve_list.insert({ {"while", 9} });
    }
    void getChar();
    void getBC();
    void Concat();
    bool isLetter();
    bool isDigital();
    int isReserve();
    void Retract();
    void InsertId();
    void InsertConst();
    void readFile();
    void writeOutput(int code, string value);
    void output();
    void run();
    void error(int pos, char ch);
    vector<pair<char, string>> get_res()
    {
        return res;
    };

    inline map<string, int> generate_var_lists() { return id_list; };

private:
    int index{};
    char ch{};
    string strToken{};
    string file{};
    string file_path;
    map<string, int> id_list{};
    int id_index = 1;
    map<string, int> const_list{};
    int const_index = 1;
    vector<string> symbols{};
    map<string, int> reserve_list = {};
    vector<pair<char, string>> res{};
};

