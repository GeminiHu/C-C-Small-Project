#include <stdio.h>
#include <string.h>

#define MAXBIT 30
#define MAXVALUE 1000

typedef struct HNode /*����ڵ�ṹ*/
{
    char data;
    double weight;
    int parent, lchild, rchild;
} HNode;
typedef struct HCode /*����������ṹ*/
{
    int bit [MAXBIT];
    int start;
} HCode;

void HuffmanCoding ( struct HNode *HT, struct HCode *HC, double *w, int n)
{
    /*w���n���ַ���Ȩֵ�������������HT�������n���ַ��Ĺ���������HC��*/
    if(n<=1)
    {
        printf("��Ϣ����̫�٣��������\n");
        return;
    }
    int m,i,j,x1,x2,start,c,f;
    double m1,m2;
    struct HNode *p;
    m=2*n-1;
    /*���������Ĺ���*/
    for(p=HT,i=0; i<n; i++, p++, w++) /*��ʼ��Ҷ�ӽڵ���Ϣ*/
    {
        p->data=i+'a';
        p->weight=*w;
        p->lchild=-1;
        p->rchild=-1;
        p->parent=-1;
    }
    for(; i<m; i++, p++)  /*��ʼ����֧�ڵ���Ϣ*/
    {
        p->weight=0;
        p->lchild=-1;
        p->rchild=-1;
        p->parent=-1;
    }
    for(i=n; i<m; i++)  /*Ѱ�Ҹ��ڵ�Ȩֵ��С�ʹ�С����������*/
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
        /*�ϲ���һ���µ�����*/
        HT[x1].parent=i;
        HT[x2].parent=i;
        HT[i].lchild=x1;
        HT[i].rchild=x2;
        HT[i].weight=m1+m2;
    }
    /*�ַ�����*/
    for(i=0; i<n; i++)
    {
        start=MAXBIT-1;
        for(c=i, f=HT[c].parent; f!=-1; c=f,f=HT[c].parent)
            if(HT[f].lchild==c) HC[i].bit[start--]=0;
            else HC[i].bit[start--]=1;
        HC[i].start=start+1; /*��¼�������ʼλ��*/
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
    printf("��Ҫ���ı������밴��1������Ҫ�����ı��밴��2��\n");
    HuffmanCoding(HT,HC,w,n);
    while(1)
    {
        printf("����������Ҫ�Ĳ���: ");
        flag= getchar();
        getchar();
        if (flag == '1')
        {
            printf("��������Ҫ���ܵ��ı��� ");
            gets(text);
            printf("���ܺ�����ģ� ");
            Coding(text,HC);
        }
        else
        {
            printf("��������Ҫ���ܵ����ģ� ");
            gets(code);
            printf("���ܺ�����ģ� ");
            Decoding(code,HT,n);
        }
        putchar('\n');
    }
    return 0;
}
