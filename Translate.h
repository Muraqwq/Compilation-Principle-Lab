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
        return a > b; // a����bʱ����true
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
    
    //���ұ����Ƿ񱻶���
    bool lookup(string name);
    // ��ӱ���
    void insert_int(string name);
    void print_res(fstream& output);
    
    // �鿴�����Ƿ���ӵ��м�
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
