//����
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
char buf[Max];//�����洢���ļ��ж�����ַ�
int k; //������ʾÿ�����ַ���λ��
int linenum = 1;
int fbegin = 0;

bool words(char s)//�����Ƿ�Ϊ��ĸ
{
    if (s <= 'z'&&s >= 'a'){ return 1; }
    if (s <= 'Z'&&s >= 'A'){ return 1; }
    return 0;
}
int digt(char d)//�����Ƿ�Ϊ����
{
    if (d >= '0'&&d <= '9'){ return 1; }
    return 0;
}
int Letter(string letter)//���Ը����ĵ���
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
int wrong(string name, int line)//������
{
    //�Դ����������ļ����д���
    ofstream errfile(errname.c_str(), ios::app);
    if (!errfile)
    {
        cout << "��ʧ�ܣ�" << endl;
        return -1;
    }
    //line�ǳ�����к�
    errfile << "****LINE:" << line << "  " << "';' is not needed or ';'is lost!" << endl;
}

int main()
{

    //�������ļ����ж�ȡ��vector
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

    //������ļ����д���
    ofstream outfile(outname.c_str());
    if (!outfile)
    {
        cout << "��ʧ�ܣ�" << endl;
        return -1;
    }

    //�Դ����������ļ����д���
    ofstream errfile(errname.c_str(), ios::app);
    if (!errfile)
    {
        cout << "��ʧ�ܣ�" << endl;
        return -1;
    }

    for (vector<string>::iterator inter = lines.begin(); inter != lines.end(); inter++)
    {
        int i = 0; //������ʾstring�е�ÿ���ַ�
        string word; //�����洢ÿ���ַ������±���ҵ�һ�е�ÿ���ַ�;
        word = *inter;
        while (word[i])
        {
            while (word[i] == ' ' || word[i] == '\t')
                i++;
            if (words(word[i])) //�����������
            {
                buf[k = 0] = word[i++];
                while (words(word[i]))
                    buf[++k] = word[i++];


                cout<< buf <<"  " << word[i] <<endl;
                outfile << setw(0) << right << buf << " " << Letter(buf) << endl;

                //��begin �� ��������ƥ��
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
//��begin �� end��ƥ��
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

                for (int l = 0; l <= Max; l++) //�������飬ʹ��Ϊ��
                {
                    buf[l] = 0;
                }
            }
            else if (digt(word[i])) //��������
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
            else  // �Ƿ��ַ�����
            {
                ofstream errfile(errname.c_str(), ios::app);
                //line�ǳ�����к�
                errfile << "****LINE:" << linenum <<':' << word[i] << " is not deffined." << endl;
                i++;
            }
        }
        linenum++;
        outfile << setw(16) << right << "EOLN" << " " << 24 << endl;//ÿ�н�β
    }
    outfile << setw(16) << right << "EOF" << " " << 25 << endl;//�ļ���β

    infile.close();
    outfile.close();
    errfile.close();
    return 0;
}
