//代码
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iomanip>

using namespace std;

string filename = "d:/code/test.pas";
string outname = "d:/code/out.dyd";
string errname = "d:/code/out.err";

const int Max = 2048;
char buf[Max];//用来存储从文件中读入的字符
int k; //用来表示每行中字符的位置
int linenum = 1;
int fbegin = 0;

bool words(char s)//测试是否为字母
{
    if (s <= 'z'&&s >= 'a'){ return 1; }
    if (s <= 'Z'&&s >= 'A'){ return 1; }
    return 0;
}
int digt(char d)//测试是否为数字
{
    if (d >= '0'&&d <= '9'){ return 1; }
    return 0;
}
int Letter(string letter)//测试给定的单词
{
    if (letter == "begin"){ return 1; }
    if (letter == "end"){ return 2; }
    if (letter == "integer"){ return 3; }
    if (letter == "if"){ return 4; }
    if (letter == "then"){ return 5; }
    if (letter == "else"){ return 6; }
    if (letter == "function"){ return 7; }
    if (letter == "read"){ return 8; }
    if (letter == "write"){ return 9; }
    return 10;
}
int wrong(string name, int line)//出错处理
{
    //对错误处理的输出文件进行处理
    ofstream errfile(errname.c_str(), ios::app);
    if (!errfile)
    {
        cout << "打开失败！" << endl;
        return -1;
    }
    //line是出错的行号
    errfile << "****LINE:" << line << "  " << "';' is not needed or ';'is lost!" << endl;
}

int main()
{

    //将输入文件按行读取到vector
    ifstream infile(filename.c_str());
    if (!infile)
    {
        cout << "can not open file!" << endl;
        return 0;
    }
    vector<string> lines;
    string line;
    while (getline(infile, line))
    {
        lines.push_back(line);
        //cout << line << endl;
    }
    infile.close();

    //对输出文件进行处理
    ofstream outfile(outname.c_str());
    if (!outfile)
    {
        cout << "打开失败！" << endl;
        return -1;
    }

    //对错误处理的输出文件进行处理
    ofstream errfile(errname.c_str(), ios::app);
    if (!errfile)
    {
        cout << "打开失败！" << endl;
        return -1;
    }

    for (vector<string>::iterator inter = lines.begin(); inter != lines.end(); inter++)
    {
        int i = 0; //用来表示string中的每个字符
        string word; //用来存储每行字符，用下标可找到一行的每个字符;
        word = *inter;
        while (word[i])
        {
            while (word[i] == ' ' || word[i] == '\t')
                i++;
            if (words(word[i])) //处理给定单词
            {
                buf[k = 0] = word[i++];
                while (words(word[i]))
                    buf[++k] = word[i++];


                cout<< buf <<"  " << word[i] <<endl;
                outfile << setw(0) << right << buf << " " << Letter(buf) << endl;

                //对begin 和 ‘；’的匹配
                if (string(buf) == "begin")
                {
                    fbegin ++;
                    cout << word[i] <<endl;
                    if(word[i] == ';')
                    {
                        wrong(errname, linenum);
                        i++;
                    }
                }
//对begin 和 end的匹配
                if (string(buf) == "end")
                {
                    cout << word[i] <<endl;
                    if(word[i] == ';')
                    {
                        if(fbegin == 1)
                        {
                            wrong(errname, linenum);
                            i++;
                        }
                    }
                    fbegin --;
                }

                for (int l = 0; l <= Max; l++) //处理数组，使其为空
                {
                    buf[l] = 0;
                }
            }
            else if (digt(word[i])) //处理数字
            {
                buf[k = 0] = word[i++];
                outfile << setw(0) << right << buf << " " << 11 << endl;
                for (int l = 0; l <= Max; l++)   {  buf[l] = 0; }
            }
            else if (word[i] == '=')
            {
                outfile << setw(0) << right << "=" << " " << 12 << endl;
                i++;
            }
            else if (word[i] == '-')
            {
                outfile << setw(0) << right << "-" << " " << 18 << endl;
                i++;
            }
            else if (word[i] == '*')
            {
                outfile << setw(0) << right << "*" << " " << 19 << endl;
                i++;
            }
            else if (word[i] == '(')
            {
                outfile << setw(0) << right << "(" << " " << 21 << endl;
                i++;
            }
            else if (word[i] == ')')
            {
                outfile << setw(0) << right << ")" << " " << 22 << endl;
                i++;
            }
            else if (word[i] == '<')
            {
                i++;
                if (word[i] == '=')
                {
                    outfile << setw(0) << right << "<=" << " " << 14 << endl;
                    i++;
                }
                else if (word[i] == '>')
                {
                    outfile << setw(0) << right << "<>" << " " << 13 << endl;
                    i++;
                }
                else
                    outfile << setw(0) << right << "<" << " " << 15 << endl;

            }
            else if (word[i] == '>')
            {
                i++;
                if (word[i] == '=')
                {
                    outfile << setw(0) << right << ">=" << " " << 0 << endl;
                    i++;
                }
                else
                    outfile << setw(0) << right << ">" << " " << 17 << endl;
            }
            else if (word[i] == ':')
            {
                i++;
                if (word[i] == '=')
                {
                    outfile << setw(0) << right << ":=" << " " << 20 << endl;
                    i++;
                }
            }
            else if (word[i] == ';')
            {
                outfile << setw(0) << right << ";" << " " << 23 << endl;
                i++;
            }
            else if (word[i] == NULL)
            {
                i++;
            }
            else  // 非法字符处理
            {
                ofstream errfile(errname.c_str(), ios::app);
                //line是出错的行号
                errfile << "****LINE:" << linenum <<':' << word[i] << " is not deffined." << endl;
                i++;
            }
        }
        linenum++;
        outfile << setw(16) << right << "EOLN" << " " << 24 << endl;//每行结尾
    }
    outfile << setw(16) << right << "EOF" << " " << 25 << endl;//文件结尾

    infile.close();
    outfile.close();
    errfile.close();
    return 0;
}
