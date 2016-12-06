#include <stdio.h>
#include <stdlib.h>

#define M 10000
#define N 11

void shortpath_DIJ(int ad[][N],int k,int e,int *pre,int *dist,int c)
{
    if(k<0||k>N||e<0||e>N)
    {
        printf("������������˲���ٴ����룡\n\n");
        return ;
    }
    int i,j,p,wm,path[N+1] = {0};
    for(i=0; i<N; i++)
    {
        pre[i] = -1;
        dist[i]=ad[k][i];
        if(dist[i]<M)
            pre[i]=k;
    }
    pre[k] = -1;
    ad[k][k]=1;
    for(j=0; j<N-1; j++)
    {
        wm=M;
        for(i=0; i<N; i++)
            if((ad[i][i]==0)&&(dist[i]<wm))
            {
                p=i;
                wm=dist[i];
            }
        if(ad[e][e]==1)  break;
        else
        {
            ad[p][p]=1;
            for(i=0; i<N; i++)
                if((ad[i][i]==0)&&(dist[p]+ad[p][i]<dist[i]))
                {
                    dist[i]=dist[p]+ad[p][i];
                    pre[i]=p;
                }
        }
    }
    path[N-1]= e;
    for(i=N-2; path[i+1]!= -1; i--)
        path[i]=pre[path[i+1]];
    for(i+=2; i<N; i++)
        printf("%d,",path[i]);
    if (dist[e]==M)
        printf("\n������յ���޿ɴ�·����\n");
    else if(dist[e]== 0)
        printf("\n������յ��غϡ�\n");
    else
    {
        if (c==1)
            printf("\n���·������Ϊ%d�ס�\n ",dist[e]);
        else
            printf("\n���·����ʱΪ%d���ӡ�\n ",dist[e]);
    }
    putchar('\n');
}

int Velocity(int i,int j,int t)
//������Ӱ��ת��Ϊ�ٶ�Ӱ�죬�Ӷ������·������ת��Ϊ���·������
{
    int v=70;
    if ((i==0&&j==1)||(i==1&&j==0))
        if (t==8||t==12||t==14||t==18)
            v=50;
    if ((i==0&&j==2)||(i==2&&j==0))
        if (t==22)
            v=60;
    return v;
}

void Build_ad(int ad[][N])
//����ռ�����ڽӾ���
{
    int i,j;
    for (i=0; i<N; i++)

        for(j=0; j<N; j++)
        {
            if (i!=j)
                ad[i][j] = M;
            else
                ad[i][j]=0;
        }
    ad[0][1]=500;
    ad[0][2]=150;
    ad[0][3]=1500;
    ad[0][4]=100;
    ad[0][7]=600;
    ad[1][4]=500;
    ad[1][7]=600;
    ad[1][10]=400;
    ad[2][3]=1400;
    ad[2][4]=200;
    ad[2][9]=600;
    ad[3][5]=200;
    ad[3][6]=200;
    ad[4][9]=500;
    ad[5][6]=200;
    ad[8][9]=100;
    ad[8][10]=300;
    for (i=0; i<N; i++)
        for(j=0; j<N; j++)
            if (ad[j][i]!= M)
                ad[i][j] = ad[j][i];

}

void Build_at(int ad[][N],int at[][N],int t)
//����ʱ������ڽӾ���
{
    int i,j;
    for (i=0; i<N; i++)
        for(j=0; j<N; j++)
            if ((ad[i][j] != M)&&(ad[i][j]!= 0))
                at[i][j]=ad[i][j]/Velocity(i,j,t);
            else
                at[i][j] = ad[i][j];
}

int main()
{
    int ad[N][N],at[N][N],k=1,e,pre[N],dist[N],c,t;
    printf("                    ��ӭʹ�õ��ӿƼ���ѧ��ˮ��У������У԰��\n\n");
    printf("*******************************************************************************\n");
    printf("У԰����Ҫ�����ı�����£�\n");
    printf("0.Ʒѧ¥ , 1.������ , 2.ͼ��� , 3.��¥ , 4.����¥ , 5.���� , 6.����������\n");
    printf("7.������ , 8.����   , 9.���Ժ , 10.��ҵ��\n");
    printf("*******************************************************************************\n\n");
    printf("�������������λ���������Ҫ�ر�ı�ţ� ");
    scanf("%d",&k);
    printf("�����������Ŀ�ĵ��������Ҫ�ر�ı�ţ� ");
    scanf("%d",&e);
    printf("ϣ�������·���밴��1���������·���밴��2���� ");
    scanf("%d",&c);
    putchar('\n');
    if (c==1)
    {
        printf("ϵͳ�������·����");
        Build_ad(ad);
        shortpath_DIJ(ad,k,e,pre,dist,c);
    }
    else
    {
        printf("�����������ڵ�ʱ�䣨0~24���������� ");
        scanf("%d",&t);
        printf("ϵͳ�������·����");
        Build_ad(ad);
        Build_at(ad,at,t);
        shortpath_DIJ(at,k,e,pre,dist,c);
    }
    printf("��л���Ե��ӿƼ���ѧ��ˮ��У������У԰�����Σ���ӭ�´��ٴ�ʹ�ã�\n\n");
    getchar();
    getchar();
    return 0;
}
