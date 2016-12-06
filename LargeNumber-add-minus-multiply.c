#include <stdio.h>
#include <math.h>
#include <string.h>

int len_a,len_b,len_x,len_y;
char a[160],b[160];
int main()
{
    while(1)
    {
        int i,j,t,flag1,flag2,flag;
        //字符串a，b存储两个数，并且计算出两个字符串的长度
        printf("请输入需要运算的两个数，每四位用逗号隔开，一个数占一行：\n");
        gets(a);
        gets(b);
        len_a = strlen(a);
        len_b = strlen(b);
        //将a，b中的逗号去掉，逆序存储在num1，num2两个数组中并记录它们的长度
        int num1[len_a-len_a/5],num2[len_b-len_b/5],*n,*m;
        for (i = len_a-1,j = 0 ; i >=0 ; i--)
        {
            if (a[i] != ','&&a[i]!='-')
            {
                num1[j] = (int)(a[i])-48;
                j++;
            }
        }
        len_x = j;
        for (i = len_b-1,j = 0 ; i >=0 ; i--)
        {
            if (b[i] != ','&&b[i]!='-')
            {
                num2[j] = (int)(b[i])-48;
                j++;
            }
        }
        len_y = j;
        //比较两个数的长度让n，m指向存储长数与短数的数组，len_x与len_y分别存储其长度
        if (len_x >= len_y)
        {
            n = num1;
            m = num2;
        }
        else
        {
            n = num2;
            m = num1;
            t = len_x;
            len_x = len_y;
            len_y = t;
        }
        //判断两个数的正负，并用flag记录
        if (a[0]=='-')
            flag1 = 0;
        else
            flag1 = 2;
        if (b[0]=='-')
            flag2 = 0;
        else
            flag2 = 1;
        flag = flag1 + flag2;//3,2,1,0分别代表++，+-，-+，--
        //调用加减乘三个函数，两个参数分别输长数和短数，输出结果
        printf("计算结果如下：\n");
        int add(int*x,int*y,int flag);
        printf("(%s) + (%s) = ",a,b);
        add(n,m,flag);
        int minus(int*x,int*y,int flag);
        printf("(%s) - (%s) = ",a,b);
        minus(n,m,flag);
        int multiply(int*x,int*y,int flag);
        printf("(%s) * (%s) = ",a,b);
        multiply(n,m,flag);
        putchar('\n');
    }
    return 0;
}

int add(int*x,int*y,int flag)
{
    int rst[len_x+1],i,j,len;
    //判断flag情况
    if (flag == 0)
        putchar('-');
    else if (flag == 2)
    {
        int minus(int*x,int*y,int flag);
        minus(x,y,3);
        return 0;
    }
    else if (flag == 1)
    {
        int minus(int*x,int*y,int flag);
        minus(x,y,0);
        return 0;
    }
    memset(rst,0,sizeof(int)*(len_x+1));//初始化存储result的数组
    //把短数的每一位与长数相加
    for (i = 0; i < len_y; i++ )
    {
        rst[i] += (*(x+i) + *(y+i));
        if (rst[i] > 9)
        {
            rst[i]-=10;
            rst[i+1]+=1;
        }
    }
    //把长数的剩余位数搬移到rst中
    for (i = len_y; i < len_x; i++)
        rst[i] += *(x+i);
    //判断结果位数
    if (rst[len_x]!=0)
        len=len_x+1;
    else
        len = len_x;
    //输出每四位有逗号的结果
    for (i = len-1,j=1; i>=0; i--,j++)
    {
        printf("%d",rst[i]);
        if ((j-len%4)%4==0&&j!=len)
            printf(",");
    }
    putchar('\n');
    return 0;
}

int minus(int*x,int*y,int flag)
{
    int rst[len_x],i,j,len,*t,p=0,d,s=0,r;
    //判断flag情况
    if (flag == 0)
        s++;
    else if (flag == 2)
    {
        int add(int*x,int*y,int flag);
        add(x,y,3);
        return 0;
    }
    else if (flag == 1)
    {
        int add(int*x,int*y,int flag);
        putchar('-');
        add(x,y,3);
        return 0;
    }
    memset(rst,0,sizeof(int)*len_x);//初始化存储result的数组
    //判断两数绝对值相减的符号,并让x,y分别指向大小数
    if (len_a < len_b)
        s++;
    if (len_x==len_y)
        for (i = len_x-1; i>=0; i--)
        {
            if (*(x+i) < *(y+i))
            {
                s++;
                t = x;
                x = y;
                y = t;
                break;
            }
        }
    //大数减小数
    for (i=0; i<len_y; i++)
    {
        r = *(x+i)-p;
        p = 0;
        d = r-*(y+i);
        if (d<0)
            p = 1;
        rst[i] = p*10+d;
    }
    if (d<0)
            rst[i]--;
    //把长数的剩余位数搬移
    for (; i<len_x; i++)
        {
            rst[i]+=*(x+i);
            if (rst[i]<0)
            {
                rst[i]=9;
                rst[i+1]--;
            }
        }
    //计算结果位数
    for (i=len_x-1; i>=0; i--)
        if (rst[i] != 0)
            break;
    len = i+1;
    if (s==1&&len!=0) putchar('-');//判断结果符号
    //输出每四位带逗号的计算结果
    if (len == 0)
        putchar('0');
    else
    {
        for (i = len-1,j=1; i>=0; i--,j++)
        {
            printf("%d",rst[i]);
            if ((j-len%4)%4==0&&j!=len)
                printf(",");
        }
    }
    putchar('\n');
    return 0;
}

int multiply(int*x,int*y,int flag)
{
    int rst[len_x+len_y],i,j,len;
    //判断flag情况
    if (flag==2||flag==1)
        putchar('-');
    memset(rst,0,sizeof(int)*(len_x+len_y));//初始化存储result的数组
    if (*y==0&&len_y==1)
        printf("0\n");
    return 0;
    //拿短数乘长数
    for (i=0; i<len_y; i++)
        for(j=0; j<len_x; j++)
            rst[i+j]+=*(y+i)*(*(x+j));
    //进位
    for (i=0; i<len_x+len_y-1; i++)
    {
        rst[i+1]+=rst[i]/10;
        rst[i]=rst[i]%10;
    }
    //判断结果位数
    if (rst[len_x+len_y-1]==0)
        len = len_x+len_y-1;
    else
        len = len_x+len_y;
    //输出每隔四个数带一个逗号的结果
    for (i = len-1,j=1; i>=0; i--,j++)
    {
        printf("%d",rst[i]);
        if ((j-len%4)%4==0&&j!=len)
            printf(",");
    }
    putchar('\n');
    return 0;
}
