#include "Reserve.h"

map<char, string> reserve_list = { { 1, "begin" },
{ 2, "end" },
{ 4, "integer" },
{ 5, "if" },
{ 6, "then" },
{ 7, "else" },
{ 8, "do" },
{ 9, "while" },
{ 10, "_" },
{ 20, ";" },
{ 23, ":=" },
{ 26, "not" },
{ 27, "\{" },
{ 28, "\}" },
{ 'I', "I" },
{ 'E', "E" },
{ 'A', "A" },
{ 'S', "S" },
{ 'C', "C" },
{ 'R', "R" },
{ 'L', "L" },
{ 'V', "S'" },
{ 'B', "B" },
{ '#', "#" },
{'M', "M"}, 
{'N', "N"}
};

set<char> nonterminals = { 'S', 'L', 'A', 'E'
            , 'M', 'N'
};
set<char> terminals = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    'I',
    'C',
    'R',
    'B', 20, 23, 26, 27, 28, '#' };