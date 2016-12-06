#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;
const int Max = 2048;

int i, j, k;//j��ʾout.dyd���ݵ�����
int level = 0;//���
int varn = 0;
int pron = 0;

char proname[16][16];//�ݴ������
char token[17];   //�൱��buffer[c].s��Ϊ�˷��ʷ��㣬����������ʹ��token����
int type;     //ͬ��
char character;   //�ַ�

ofstream errfile("d:/code/out1.err", ios::app);
ofstream varfile("d:/code/out.var");
ofstream profile("d:/code/out.pro");
ofstream dysfile("d:/code/out.dys");

struct buffer//�����洢��out.dyd�ļ����������
{
    char s[17];
    int type;//�ֱ�
}buffer[Max];

struct Var{
    char vname[16];//������
    char vproc[16];//��������
    int vkind;//���� 0 ���� 1 �β�
    char vtype[10];//��������
    int vlev;//�������
    int vadr;//�����ڱ������е�λ��
};


struct Pro{
    char pname[16];//������
    char ptype[10];//��������
    int plev;      //���̲��
    int fadr;      //��һ�������ڱ������е�λ��
    int ladr;      //���һ�������ڱ������е�λ��
};

Var var[16];//��������
Pro pro[16];//��������

void addToVar(int kind){
    int check = 0;
    for (int i = 0; i < 16; i++){//�����������û�д˱�������������������ԡ�
        if (strcmp(var[i].vname, token) == 0 && var[i].vkind == kind)
            check = 1;
        if (strcmp(pro[i].pname, token) == 0 || strcmp(proname[i], token) == 0)
            check = 1;
    }

    if (check == 0){
        memset(var[varn].vname, 0, sizeof(var[varn].vname));

        strcat(var[varn].vname, token);

        //�ҳ��������������̡�
        if (level == 1)
            strcat(var[varn].vproc, "main process");
        else{
            for (int i = pron; i >= 0; i--){
                if (pro[i].plev == level)
                    strcat(var[varn].vproc, pro[i].pname);
            }
        }
        var[varn].vkind = kind;
        strcat(var[varn].vtype, "integer");
        var[varn].vlev = level;
        var[varn].vadr = varn;
        varn++;
    }

}

void addToPro(char* proname, int f, int l){
    strcat(pro[pron].pname, proname);
    strcat(pro[pron].ptype, "integer");
    pro[pron].plev = level - 1;
    pro[pron].fadr = f;
    pro[pron].ladr = l;
}

//������һ���ַ�
void advance(){
    while (buffer[j].type == 24)
        j++;
    memset(token, 0, sizeof(token));
    strcat(token, buffer[j].s);
    type = buffer[j].type;
    j++;
}

void error(){
    errfile << "Line NO." << j - 1 << " has Syn_error: " << endl;
}

void SP();
void DST();
void DST1();
void DS();
void DS1();
void V();
void ID();
void PM();
void FB();
void EST();
void EST1();
void ES();
void RS();
void WS();
void AS();
void AE();
void AE1();
void IT();
void IT1();
void F();
void FC();
void CS();
void CE();
void RO();

void P(){
    SP();
}

void SP(){
    if (type == 1){
        level++;
        advance();
        DST();

        EST();
        if (type == 2)
            level--;
        else error();
    }
    else error();
}
//˵������
void DST(){
    DS();
    DST1();
}
//���ڡ���������˵����仹�ǽ���ִ���������⣬
//������£�������һ���ַ������integer����˵����䣬�����˳�˵������
void DST1(){
    if (type == 23){
        advance();
        if (type == 3)
        {
            DS();
            DST1();
        }
    }
}
//˵�����
void DS(){
    if (type == 3){
        advance();
        DS1();
    }
    else error();
}
//˵����䣬����ǹ��̵Ļ�����ӵ����̱����ѹ��̵��β���ӵ�������
void DS1(){
    if (type == 7){//����˵��
        advance();

        memset(proname[level], 0, sizeof(type));
        strcat(proname[level], token);

        int f = 0;
        int l = 0;

        ID();
        if (type == 21){
            advance();
            f = varn;

            PM();
            if (type == 22){
                advance();
                if (type == 23){
                    advance();
                    FB();
                    l = varn - 1;
                }
                else error();
            }
            else error();
        }
        else error();

        addToPro(proname[level], f, l);//��������ӵ����̱�
    }
    else V();//����˵��
}
//����
void V(){
    addToVar(0);
    ID();
}
//��ӵ�������
void ID(){
    if (type == 10)
        advance();
    else error();
}
//����
void PM(){
    //varn--;
    addToVar(1);
    V();
}
//������
void FB(){
    if (type == 1){
        level++;
        advance();
        DST();

        EST();
        if (type == 2){
            level--;
            advance();
        }
        else error();
    }
    else error();
}

//ִ������
void EST(){
    ES();
    EST1();
}

void EST1(){
    if (type == 23){
        advance();
        ES();
        EST1();
    }
}

//ִ�����
void ES(){
    if (type == 8){
        RS();
    }
    else if (type == 9){
        WS();
    }
    else if (type == 4){
        CS();
    }
    else{
        AS();
    }
}

//��
void RS(){
    advance();
    if (type == 21){
        advance();
        V();
        if (type == 22)
            advance();
        else error();
    }
    else error();
}

//д
void WS(){
    advance();
    if (type == 21){
        advance();
        V();
        if (type == 22)
            advance();
        else error();
    }
    else error();
}

//��ֵ
void AS(){
    V();
    if (type == 20){
        advance();
        AE();
    }
    else error();
}

//�������ʽ
void AE(){
    IT();
    AE1();
}

void AE1(){
    if (type == 18){
        advance();
        IT();
        AE1();
    }
}

//��
void IT(){
    F();
    IT1();
}

void IT1(){
    if (type == 19){
        advance();
        F();
        IT1();
    }
}

//����,������������
void F(){
    if (type == 10){         //�С��������Ǻ������ã���������ͨ����
        advance();

        if (type == 21){     //����������
            advance();
            AE();
            if (type == 22){
                advance();
            }
        }

    }
    else if (type == 11){
        advance();
    }
}

//��������
void FC(){
    if (type == 10){
        advance();
        if (type == 21){
            advance();
            AE();
            if (type == 22){
                advance();
            }
            else error();
        }
        else error();
    }
    else error();
}

void CS(){
    advance();
    CE();
    if (type == 5){
        advance();
        ES();
        if (type == 6){
            advance();
            ES();
        }
        else error();
    }
    else error();
}

void CE(){
    AE();
    RO();
    AE();
}

void RO(){
    if (type == 12) advance();
    else if (type == 13)    advance();
    else if (type == 14)    advance();
    else if (type == 15)    advance();
    else if (type == 16)    advance();
    else if (type == 17)    advance();
    else if (type == 20)    advance();
    else error();
}

int main()
{
    FILE*in = fopen("d:/code/out.dyd", "r");
    int c = 0;
    while (~fscanf(in, "%s%d", buffer[c].s, &buffer[c].type))
        c++;
    fclose(in);

    ofstream firsterr("d:/code/out.err");
    if (!firsterr)
    {
        cout << "open failed." << endl;
        //exit(1);
    }
    if (firsterr.eof())
    {
        cout << "Something is wrong in the first step." << endl;
        return 0;
    }

    j = 0;
    advance();
    P();

    for (int i = 0; i < varn; i++){
        varfile << var[i].vname << " " << var[i].vproc << " " << var[i].vkind << " " << var[i].vlev << " " << var[i].vadr << endl;
        cout << var[i].vname << endl;
    }
    for (int i = 0; i <= pron; i++){
        profile << pro[i].pname << " " << pro[i].ptype << " " << pro[i].plev << " " << pro[i].fadr << " " << pro[i].ladr << endl;
    }

    varfile.close();
    profile.close();
    errfile.close();
    return 0;

}
