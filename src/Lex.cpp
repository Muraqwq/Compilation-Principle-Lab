#include "Lex.h"

void Lex::readFile()
{
    ifstream infile(file_path);

    std::stringstream buffer;
    buffer << infile.rdbuf();
    file = buffer.str();

    // �ر��ļ�
    infile.close();

    return;
};

void Lex::getChar()
{
    if (index < file.size())
        ch = file[index++];
    else
        index++;
};

void Lex::getBC()
{
    while (ch == ' ' or ch == '\n')
        getChar();
};

void Lex::Concat()
{
    strToken.push_back(ch);
};

bool Lex::isLetter()
{
    return isalpha(ch);
};

bool Lex::isDigital()
{
    return isdigit(ch);
};

int Lex::isReserve()
{
    if (reserve_list.find(strToken) == reserve_list.end())
    {
        return -1;
    }
    else
    {
        return reserve_list.find(strToken)->second;
    }
};

void Lex::Retract()
{
    index--;
    ch = 0;
};

void Lex::InsertId()
{
    id_list.insert({strToken, id_index});
    id_index++;
};
void Lex::InsertConst()
{
    const_list.insert({strToken, const_index});
    const_index++;
};

void Lex::run()
{

    readFile();
    // cout << "�ʷ�����������..." << endl;
    while (index < file.size() - 1)
    {
        // cout << "��ǰλ�ã�" << index << endl;
        int code;
        string value;
        strToken = "";
        getChar();
        getBC();
        if (isLetter())
        {
            while (index <= file.size() and (isLetter() or isDigital()))
            {
                Concat();
                getChar();
            }

            Retract();
            code = isReserve();
            if (code == -1)
            {
                InsertId();
                writeOutput(VAR, strToken);
            }
            else
            {
                writeOutput(code, "-");
            }
        }
        else if (isDigital())
        {
            while (isDigital())
            {
                Concat();
                getChar();
            }

            Retract();
            InsertConst();
            writeOutput(INT, strToken);
        }
        else if (ch == '+')
        {
            writeOutput(PLUS, "-");
        }
        else if (ch == '*')
        {
            writeOutput(TIMES, "*");
        }
        else if (ch == '(')
        {
            writeOutput(LEFT_PAREN, "-");
        }
        else if (ch == ')')
        {
            writeOutput(RIGHT_PAREN, "-");
        }
        else if (ch == '>')
        {
            writeOutput(GREATER, "-");
        }
        else if (ch == '=')
        {
            writeOutput(EQUAL, "-");
        }
        else if (ch == '<')
        {
            writeOutput(LESS, "-");
        }
        else if (ch == ';')
        {
            writeOutput(SEMICOLON, "-");
        }
        else if (ch == ',')
        {
            writeOutput(COMMA, "-");
        }
        else if (ch == '!')
        {
            writeOutput(NOT, "-");
        }
        else if (ch == ':')
        {
            Concat();
            getChar();
            if (ch == '=')
            {
                Concat();
                writeOutput(ASSIGN, "-");
            }
            else
            {
                Retract();
                writeOutput(COLON, "-");
            }
        }
        else if (ch == '&')
        {
            Concat();
            getChar();
            if (ch == '&')
            {
                Concat();
                writeOutput(AND, "-");
            }
            else
            {
                Retract();
                error(index, ch);
                return;
            }
        }
        else if (ch == '|')
        {
            Concat();
            getChar();
            if (ch == '|')
            {
                Concat();
                writeOutput(OR, "-");
            }
            else
            {
                Retract();
                error(index, ch);
                return;
            }
        }
        else if (ch == '{')
        {
            writeOutput(LEFT_CURLY_BRACE, "-");
        }
        else if (ch == '}')
        {
            writeOutput(RIGHT_CURLY_BRACE, "-");
        }
        else
        {
            continue;
        }
    }

    output();
};

void Lex::writeOutput(int code, string value)
{
    symbols.push_back("( " + to_string(code) + ", " + value + " )");
    res.push_back({code, value});
};

void Lex::output()
{
    string output_path = "output.txt";
    std::ofstream outfile(output_path);
    if (!outfile.is_open())
    {
        std::cerr << "�޷����ļ�: " << std::endl;
        return;
    }
    int index = 0;
    for (auto item : symbols)
    {
        outfile << "( " + to_string(++index) + " )   " + item << std::endl;
    }

    outfile.close();
};

void Lex::error(int pos, char ch)
{
    cerr << "�ʷ��������� λ�ã� " << to_string(pos) << ", ���ţ�" << ch << endl;
}