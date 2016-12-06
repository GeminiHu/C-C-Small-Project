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

int i, j, k;//j表示out.dyd内容的行数
int level = 0;//层次
int varn = 0;
int pron = 0;

char proname[16][16];//暂存过程名
char token[17];   //相当于buffer[c].s，为了访问方便，在主程序中使用token代替
int type;     //同上
char character;   //字符

ofstream errfile("d:/code/out1.err", ios::app);
ofstream varfile("d:/code/out.var");
ofstream profile("d:/code/out.pro");
ofstream dysfile("d:/code/out.dys");

struct buffer//用来存储从out.dyd文件读入的内容
{
    char s[17];
    int type;//种别
}buffer[Max];

struct Var{
    char vname[16];//变量名
    char vproc[16];//所属过程
    int vkind;//分类 0 变量 1 形参
    char vtype[10];//变量类型
    int vlev;//变量层次
    int vadr;//变量在变量表中的位置
};


struct Pro{
    char pname[16];//过程名
    char ptype[10];//过程类型
    int plev;      //过程层次
    int fadr;      //第一个变量在变量表中的位置
    int ladr;      //最后一个变量在变量表中的位置
};

Var var[16];//变量名表
Pro pro[16];//过程名表

void addToVar(int kind){
    int check = 0;
    for (int i = 0; i < 16; i++){//如果变量表中没有此变量则加入变量表。否则忽略。
        if (strcmp(var[i].vname, token) == 0 && var[i].vkind == kind)
            check = 1;
        if (strcmp(pro[i].pname, token) == 0 || strcmp(proname[i], token) == 0)
            check = 1;
    }

    if (check == 0){
        memset(var[varn].vname, 0, sizeof(var[varn].vname));

        strcat(var[varn].vname, token);

        //找出，变量所属过程。
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

//读出下一个字符
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
//说明语句表
void DST(){
    DS();
    DST1();
}
//关于‘；’后是说明语句还是进入执行语句的问题，
//解决如下：读入下一个字符如果是integer则是说明语句，否则退出说明语句表。
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
//说明语句
void DS(){
    if (type == 3){
        advance();
        DS1();
    }
    else error();
}
//说明语句，如果是过程的话，添加到过程表，并把过程的形参添加到变量表
void DS1(){
    if (type == 7){//函数说明
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

        addToPro(proname[level], f, l);//将过程添加到过程表
    }
    else V();//变量说明
}
//变量
void V(){
    addToVar(0);
    ID();
}
//添加到变量表
void ID(){
    if (type == 10)
        advance();
    else error();
}
//参数
void PM(){
    //varn--;
    addToVar(1);
    V();
}
//函数体
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

//执行语句表
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

//执行语句
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

//读
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

//写
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

//赋值
void AS(){
    V();
    if (type == 20){
        advance();
        AE();
    }
    else error();
}

//算数表达式
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

//项
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

//因子,包含函数调用
void F(){
    if (type == 10){         //有“（”则是函数调用，否则是普通变量
        advance();

        if (type == 21){     //处理函数调用
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

//函数调用
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
