#include "Translate.h"

Translate::Translate()
{
    for (int i = 0; i < 50; i++)
    {
        temp.push_back("T" + to_string(i + 1));
    }
};
void Translate::backpatch(vector<int>& list, int seqno)
{
    for (int quaternary : list)
    {
        quaternary_list[quaternary - init_no][3] = to_string(seqno);
    }
}

vector<int> Translate::merge(const vector<vector<int>>& lists)
{
    vector<int> tmp{};
    for (auto& list : lists)
    {
        tmp.insert(tmp.end(), list.begin(), list.end());
        sort(tmp.begin(), tmp.end(), [](int a, int b)
            {
                return a > b; // a大于b时返回true
            });
    }

    return tmp;
}

void Translate::emit(vector<string> input)
{
    quaternary_list.emplace_back(input);
    std::cout << "(" << input[0] << "," << input[1] << ", " << input[2] << ", " << input[3] << " )" << endl;
};

string Translate::getTmp(int index)
{
    return temp[index];
}

void Translate::new_temp(E* e)
{
    e->place.index = ++temp_num;
    
}

int Translate::next_seq()
{
    return quaternary_list.size() + init_no;
}

bool Translate::lookup(string name)
{
    

    return !(variables.find(name) == variables.end());
      
}

void Translate::insert_int(string name)
{
    variables.insert({ name, nullptr });
    
}

void Translate::print_res(fstream& output)
{
    int index = 0;
    for (auto item : quaternary_list) {
        output <<init_no + index++<< "(" <<item[0] << "," << item[1] << ", " << item[2] << ", " << item[3] << " )" << endl;
        
    }
}
