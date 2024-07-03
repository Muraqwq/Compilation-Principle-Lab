#pragma once

#include "stdio.h"
#include "iostream"
#include "Symbol.h"
#include "vector"
#include "map"
#include "fstream"
#include "algorithm"
using namespace std;

struct Greater
{
    bool operator()(int a, int b) const
    {
        return a > b; // a大于b时返回true
    }
};

class Translate
{
public:
    Translate();
    int next_seq();
    void backpatch(vector<int>& list, int seqno);
    vector<int> merge(const vector<vector<int>>& lists);
    void emit(vector<string> input);
    string getTmp(int index);
    void new_temp(E* e);
    
    //查找变量是否被定义
    bool lookup(string name);
    // 添加变量
    void insert_int(string name);
    void print_res(fstream& output);
    
    // 查看变量是否被添加到中间
    /*int is_assigned(string var){
        if (tmp_assign_list.find(var) != tmp_assign_list.end()) {
            return tmp_assign_list[var];
        }
        else return -1;
    }*/
    //void assign_tmp(string var, int index) { tmp_assign_list.insert({ var, temp_num }); }
    map<string, E*> variables{};
private:
    int init_no = 100;
    vector<vector<string>> quaternary_list{};
    vector<string> temp;
    
    //map<string, int> tmp_assign_list{};
    
    int temp_num{};
};
