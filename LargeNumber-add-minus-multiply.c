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
        //�ַ���a��b�洢�����������Ҽ���������ַ����ĳ���
        printf("��������Ҫ�������������ÿ��λ�ö��Ÿ�����һ����ռһ�У�\n");
        gets(a);
        gets(b);
        len_a = strlen(a);
        len_b = strlen(b);
        //��a��b�еĶ���ȥ��������洢��num1��num2���������в���¼���ǵĳ���
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
        //�Ƚ��������ĳ�����n��mָ��洢��������������飬len_x��len_y�ֱ�洢�䳤��
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
        //�ж�������������������flag��¼
        if (a[0]=='-')
            flag1 = 0;
        else
            flag1 = 2;
        if (b[0]=='-')
            flag2 = 0;
        else
            flag2 = 1;
        flag = flag1 + flag2;//3,2,1,0�ֱ����++��+-��-+��--
        //���üӼ����������������������ֱ��䳤���Ͷ�����������
        printf("���������£�\n");
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
    //�ж�flag���
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
    memset(rst,0,sizeof(int)*(len_x+1));//��ʼ���洢result������
    //�Ѷ�����ÿһλ�볤�����
    for (i = 0; i < len_y; i++ )
    {
        rst[i] += (*(x+i) + *(y+i));
        if (rst[i] > 9)
        {
            rst[i]-=10;
            rst[i+1]+=1;
        }
    }
    //�ѳ�����ʣ��λ�����Ƶ�rst��
    for (i = len_y; i < len_x; i++)
        rst[i] += *(x+i);
    //�жϽ��λ��
    if (rst[len_x]!=0)
        len=len_x+1;
    else
        len = len_x;
    //���ÿ��λ�ж��ŵĽ��
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
    //�ж�flag���
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
    memset(rst,0,sizeof(int)*len_x);//��ʼ���洢result������
    //�ж���������ֵ����ķ���,����x,y�ֱ�ָ���С��
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
    //������С��
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
    //�ѳ�����ʣ��λ������
    for (; i<len_x; i++)
        {
            rst[i]+=*(x+i);
            if (rst[i]<0)
            {
                rst[i]=9;
                rst[i+1]--;
            }
        }
    //������λ��
    for (i=len_x-1; i>=0; i--)
        if (rst[i] != 0)
            break;
    len = i+1;
    if (s==1&&len!=0) putchar('-');//�жϽ������
    //���ÿ��λ�����ŵļ�����
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
    //�ж�flag���
    if (flag==2||flag==1)
        putchar('-');
    memset(rst,0,sizeof(int)*(len_x+len_y));//��ʼ���洢result������
    if (*y==0&&len_y==1)
        printf("0\n");
    return 0;
    //�ö����˳���
    for (i=0; i<len_y; i++)
        for(j=0; j<len_x; j++)
            rst[i+j]+=*(y+i)*(*(x+j));
    //��λ
    for (i=0; i<len_x+len_y-1; i++)
    {
        rst[i+1]+=rst[i]/10;
        rst[i]=rst[i]%10;
    }
    //�жϽ��λ��
    if (rst[len_x+len_y-1]==0)
        len = len_x+len_y-1;
    else
        len = len_x+len_y;
    //���ÿ���ĸ�����һ�����ŵĽ��
    for (i = len-1,j=1; i>=0; i--,j++)
    {
        printf("%d",rst[i]);
        if ((j-len%4)%4==0&&j!=len)
            printf(",");
    }
    putchar('\n');
    return 0;
}
