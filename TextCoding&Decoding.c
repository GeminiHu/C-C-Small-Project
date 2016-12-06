#include <stdio.h>
#include <string.h>

#define MAXBIT 30
#define MAXVALUE 1000

typedef struct HNode /*定义节点结构*/
{
    char data;
    double weight;
    int parent, lchild, rchild;
} HNode;
typedef struct HCode /*哈夫曼编码结构*/
{
    int bit [MAXBIT];
    int start;
} HCode;

void HuffmanCoding ( struct HNode *HT, struct HCode *HC, double *w, int n)
{
    /*w存放n个字符的权值，构造哈夫曼树HT，并求出n个字符的哈夫曼编码HC。*/
    if(n<=1)
    {
        printf("信息种类太少，无需编码\n");
        return;
    }
    int m,i,j,x1,x2,start,c,f;
    double m1,m2;
    struct HNode *p;
    m=2*n-1;
    /*哈夫曼树的构造*/
    for(p=HT,i=0; i<n; i++, p++, w++) /*初始化叶子节点信息*/
    {
        p->data=i+'a';
        p->weight=*w;
        p->lchild=-1;
        p->rchild=-1;
        p->parent=-1;
    }
    for(; i<m; i++, p++)  /*初始化分支节点信息*/
    {
        p->weight=0;
        p->lchild=-1;
        p->rchild=-1;
        p->parent=-1;
    }
    for(i=n; i<m; i++)  /*寻找根节点权值最小和次小的两棵子树*/
    {
        m1=m2=MAXVALUE;
        x1=x2=0;
        for(j=0; j<i; j++)
        {
            if (HT[j].parent==-1 &&HT[j].weight<m1)
            {
                m2=m1;
                x2=x1;
                m1=HT[j].weight;
                x1=j;
            }
            else if( HT[j].parent==-1 && HT[j].weight<m2)
            {
                m2=HT[j].weight;
                x2=j;
            }
        }
        /*合并成一棵新的子树*/
        HT[x1].parent=i;
        HT[x2].parent=i;
        HT[i].lchild=x1;
        HT[i].rchild=x2;
        HT[i].weight=m1+m2;
    }
    /*字符编码*/
    for(i=0; i<n; i++)
    {
        start=MAXBIT-1;
        for(c=i, f=HT[c].parent; f!=-1; c=f,f=HT[c].parent)
            if(HT[f].lchild==c) HC[i].bit[start--]=0;
            else HC[i].bit[start--]=1;
        HC[i].start=start+1; /*记录编码的起始位置*/
    }
}
void Coding(char *text,struct HCode *HC)
{
    text = strlwr(text);
    int i=0,k,j;
    while (text[i]!=NULL)
    {
        k = text[i]-'a';
        for (j= HC[k].start; j<MAXBIT; j++)
            printf("%d",HC[k].bit[j]);
        i++;
    }
    putchar('\n');
}

void Decoding(char *code,struct HNode *HT,int n)
{
    int i=0;
    struct HNode *track;
    while (code[i]!= NULL)
    {
        track = &HT[2*n-2];
        while (track->lchild!=-1||track->rchild!=-1)
        {
            if (code[i]=='0') track = &HT[track->lchild];
            else track = &HT[track->rchild];
            i++;
        }
        putchar(track->data);
    }
    putchar('\n');
}

int main()
{
    double w[26]= {8.56,2.12,4.74,3.13,11.42,1.47,2.3,2.76,7.94,0.15,0.84,5.52,3.22,6.41,7.12,3.27,0.23,7.51,5.55,7.46,3.66,1.07,0.94,0.35,2.0,0.24};
    int n = 26;
    struct HNode *HT;
    struct HCode *HC;
    HT=(HNode *) malloc((2*n-1)*sizeof(struct HNode));
    HC=(HCode *) malloc((n)*sizeof(struct HCode));
    char text[300],code[300],flag;
    printf("需要给文本加密请按“1”，需要解密文本请按“2”\n");
    HuffmanCoding(HT,HC,w,n);
    while(1)
    {
        printf("请输入您需要的操作: ");
        flag= getchar();
        getchar();
        if (flag == '1')
        {
            printf("请输入需要加密的文本： ");
            gets(text);
            printf("加密后的密文： ");
            Coding(text,HC);
        }
        else
        {
            printf("请输入需要解密的密文： ");
            gets(code);
            printf("解密后的明文： ");
            Decoding(code,HT,n);
        }
        putchar('\n');
    }
    return 0;
}
