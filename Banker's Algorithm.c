#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
# define m 50

int no1;  //������
int no2;  //��Դ��
int r;
int allocation[m][m],need[m][m],available[m],max[m][m];
char name1[m],name2[m];                               //����ȫ�ֱ���
void main()
{
	void check();
	void print();
	int i,j,p=0,q=0;
	char c;
	int request[m],allocation1[m][m],need1[m][m],available1[m];
	printf("**************��һ����ѧ�ţ�2013100103011�������м��㷨ʵ��************\n");
	printf("�������������:\n");
	scanf("%d",&no1);
	printf("��������Դ������:\n");
	scanf("%d",&no2);
    printf("������Max����:\n");
	for(i=0;i<no1;i++)
		for(j=0;j<no2;j++)
			scanf("%d",&max[i][j]);   //������֪���������Դ������

	printf("������Allocation����:\n");
	for(i=0;i<no1;i++)
		for(j=0;j<no2;j++)
			scanf("%d",&allocation[i][j]);  //������֪�Ľ����ѷ������Դ��

	for(i=0;i<no1;i++)
		for(j=0;j<no2;j++)
			need[i][j]=max[i][j]-allocation[i][j]; //���������������������need�����ֵ

	printf("������Available����\n");
	for(i=0;i<no2;i++)
		scanf("%d",&available[i]);       //������֪�Ŀ�����Դ��

	print();  //�����֪����
	check();  //���T0ʱ����֪�����İ�ȫ״̬

	if(r==1)  //�����ȫ��ִ�����´���
	{
		do{
q=0;
            p=0;
printf("\n������������Դ�Ľ��̺�(0~4)��\n");
			for(j=0;j<=10;j++)
			{
				scanf("%d",&i);
				if(i>=no1)
				{
					printf("����������������룺\n");
				    continue;
				}
				else break;
			}
			printf("\n������ý������������Դ��request[j]:\n");
			for(j=0;j<no2;j++)
				scanf("%d",&request[j]);
			for(j=0;j<no2;j++)
				if(request[j]>need[i][j]) p=1;
				//�ж������Ƿ񳬹��ý�������Ҫ����Դ��
				if(p)
					printf("������Դ�����ý�����Դ������������ʧ�ܣ�\n");
				else
				{
					for(j=0;j<no2;j++)
					if(request[j]>available[j]) q=1;
//�ж������Ƿ񳬹�������Դ��
					if(q)
						printf("û����������Դ���䣬����ʧ�ܣ�\n");

					else                             //������������
					{
						for(j=0;j<no2;j++)
						{
							available1[j]=available[j];
							allocation1[i][j]=allocation[i][j];
							need1[i][j]=need[i][j];
					//����ԭ�ѷ������Դ��������Ҫ����Դ���Ϳ��õ���Դ��

							available[j]=available[j]-request[j];
							allocation[i][j]+=request[j];
							need[i][j]=need[i][j]-request[j];
                            //ϵͳ���԰���Դ���������Ľ���
						}
						print();
						check();     //�������İ�ȫ��
						if(r==0)   //��������ϵͳ����ȫ
						{
							for(j=0;j<no2;j++)
							{
								available[j]=available1[j];
							    allocation[i][j]=allocation1[i][j];
							    need[i][j]=need1[i][j];
                    //��ԭ�ѷ������Դ��������Ҫ����Դ���Ϳ��õ���Դ��
							}
							printf("���ط���ǰ��Դ��\n");
							print();
						}
					}
				}printf("\n�㻹Ҫ����������Y or N ?\n");
				//�ж��Ƿ����������Դ����
					c=getche();
		}while(c=='y'||c=='Y');
	}

}

void check()   //��ȫ�㷨����
{
	int k,f,v=0,i,j;
	int work[m],a[m];
	bool finish[m];
	r=1;
	for(i=0;i<no1;i++)
		finish[i]=false;   // ��ʼ�����̾�û�õ��㹻��Դ�������
	for(i=0;i<no2;i++)
	    work[i]=available[i];//work[i]��ʾ���ṩ���̼������еĸ�����Դ��
	k=no1;
	do{
		for(i=0;i<no1;i++)
		{
			if(finish[i]==false)
			{
				f=1;
				for(j=0;j<no2;j++)
					if(need[i][j]>work[j])
						f=0;
					if(f==1)      //�ҵ���û�������������С�ڿ��ṩ���̼������е���Դ���Ľ���
					{
						finish[i]=true;
						a[v++]=i;   //��¼��ȫ���к�
						for(j=0;j<no2;j++)
							work[j]+=allocation[i][j];  //�ͷŸý����ѷ������Դ
					}
			}
		}
		k--;      //ÿ���һ�����̷��䣬δ��ɵĽ������ͼ�1
	}while(k>0);
	f=1;
	for(i=0;i<no1;i++)    //�ж��Ƿ����еĽ��̶����
	{
		if(finish[i]==false)
		{
			f=0;
			break;
		}
	}
	if(f==0)       //���н���û��ɣ���Ϊ����ȫ״̬
	{
		printf("ϵͳ���ڲ���ȫ״̬��");
		r=0;
	}
	else
	{
		printf("\nϵͳ��ǰΪ��ȫ״̬����ȫ����Ϊ��\n");
		for(i=0;i<no1;i++)
			printf("p%d  ",a[i]);  //�����ȫ����
	}

}

void print()  //�������
{
	int i,j;
	printf("\n");
	printf("*************��ʱ����Դ�������*********************\n");
	printf("������/��   |   Max     | Allocation  |     Need    |\n");
    for (i = 0; i < no1; i++)
		{
			printf("   p%d/%d      ",i,i);

         	for (j = 0; j < no2; j++)
            	{printf("%d   ",max[i][j]);}

			for (j = 0; j < no2; j++)
			    {printf(" %d   ",allocation[i][j]);}

			for (j = 0; j < no2; j++)
			    {printf(" %d   ",need[i][j]);}

			printf("\n");
		}
	    printf("\n");
		printf("������Դ�����õ���Դ��Ϊ:");
		for (j = 0; j < no2; j++)
		    {printf(" %d",available[j]);}
		printf("\n");
}
