#include <stdio.h>
#include <stdlib.h>

#define M 10000
#define N 11

void shortpath_DIJ(int ad[][N],int k,int e,int *pre,int *dist,int c)
{
    if(k<0||k>N||e<0||e>N)
    {
        printf("输入标号有误，请核查后再次输入！\n\n");
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
        printf("\n起点与终点间无可达路径。\n");
    else if(dist[e]== 0)
        printf("\n起点与终点重合。\n");
    else
    {
        if (c==1)
            printf("\n最短路径距离为%d米。\n ",dist[e]);
        else
            printf("\n最快路径耗时为%d分钟。\n ",dist[e]);
    }
    putchar('\n');
}

int Velocity(int i,int j,int t)
//将流量影响转化为速度影响，从而将最短路径问题转化为最快路径问题
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
//构造空间距离邻接矩阵
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
//构造时间距离邻接矩阵
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
    printf("                    欢迎使用电子科技大学清水河校区数字校园！\n\n");
    printf("*******************************************************************************\n");
    printf("校园中主要建筑的标号如下：\n");
    printf("0.品学楼 , 1.宿舍区 , 2.图书馆 , 3.主楼 , 4.科研楼 , 5.南门 , 6.南门体育馆\n");
    printf("7.体育场 , 8.西门   , 9.电科院 , 10.商业街\n");
    printf("*******************************************************************************\n\n");
    printf("请输入距你现在位置最近的主要地标的标号： ");
    scanf("%d",&k);
    printf("请输入距离你目的地最近的主要地标的标号： ");
    scanf("%d",&e);
    printf("希望走最短路径请按“1”，走最快路径请按“2”： ");
    scanf("%d",&c);
    putchar('\n');
    if (c==1)
    {
        printf("系统建议最短路径：");
        Build_ad(ad);
        shortpath_DIJ(ad,k,e,pre,dist,c);
    }
    else
    {
        printf("请输入你现在的时间（0~24的整数）： ");
        scanf("%d",&t);
        printf("系统建议最快路径：");
        Build_ad(ad);
        Build_at(ad,at,t);
        shortpath_DIJ(at,k,e,pre,dist,c);
    }
    printf("感谢您对电子科技大学清水河校区数字校园的信任，欢迎下次再次使用！\n\n");
    getchar();
    getchar();
    return 0;
}
