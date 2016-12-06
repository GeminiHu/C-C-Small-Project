#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib");

struct doc_type
{
	char *suffix;
	char *type;
};
//����Ŀɹ���ѯ���ļ�����
struct doc_type file_type[]=
{
	{"html","html"},
	{"jpg","jpg"},
	{"txt","txt"},
	{"ico","ico"},
	{NULL,NULL}
};

//�ɹ��ҵ���200 OK��Ӧ����
char *http_res_mes="HTTP/1.1 200 OK\r\n"//404 NOT FOUND
	"Accept-Ranges: bytes\r\n"
	"Content-Length: %d\r\n"
	"Connection: close\r\n"
	"Content-type: %s\r\n\r\n";

/***************************************
*
*�������ܣ������ļ���׺���Ҷ�Ӧ��Content-Type
*����suffixΪ�����ļ���׺��
*
****************************************/
char *http_suffix2type(const char *suffix)
{
	struct doc_type *type;
	for (type=file_type;type->suffix;type++)
	{
		if (strcmp(type->suffix,suffix)==0)
			return type->type;
	}
	return 0;
}

/***************************************
*�������ܣ�����HTTP���������У��õ��ļ����ͺ�׺��
*bufΪ�ͻ��˷��ط���������Ϣ
*file_nameΪ�����ļ���
*suffixΪ�����ļ���׺��
****************************************/
void http_request(char *buf,int buflen,char *file_name,char *suffix)
{
    int length=0;
	//��ʼ�㡢�����㡢ƫ�Ƶ�
    char *begin,*end,*bias;

	//�����е��ص㣬�����ո�֮��ΪURL

    //����URL�Ŀ�ʼλ��
    begin=strchr(buf,' ');
    begin+=1;

    //���� URL �Ľ���λ��
    end=strchr(begin,' ');
    *end=0;

    bias=strrchr(begin,'/');
    length=end-bias;

    //�ҵ��ļ����Ŀ�ʼλ��
    if((*bias=='/')||(*bias=='\\'))
    {
        bias++;
        length--;
    }

    // �õ��ļ���
    if(length>0)
    {
        memcpy(file_name,bias,length);
        file_name[length] = 0;

        begin=strchr(file_name,'.');
        if (begin)
            strcpy(suffix,begin + 1);
    }
}

/***************************************
*
*�������ܣ���������ͻ��˷���HTTP��Ӧ
*socketΪ�ͻ��˵��׽���
*bufΪ�ͻ��˷��͵���������Ϣ
*
****************************************/
int http_res(SOCKET socket,char *buf,int buf_len)
{
	char *type;
	char read_buf[1024];
	char http_hdr[1024];
	char file_name[256]="2013060107017.html",suffix[16]="html";
	int read_len,file_len,hdr_len,send_len;
	FILE *res_file;

	//�õ��ļ�����׺
	http_request(buf,buf_len,file_name,suffix);
	res_file=fopen(file_name,"rb+");

	//�ļ������ڣ�404����
	if(res_file==NULL)
	{
		char *http_res_mes="HTTP/1.1 404 Not Found\r\n";
		hdr_len=sprintf(http_hdr,http_res_mes);
		send_len=send(socket,http_hdr,hdr_len,0);
		printf("%s\n",http_hdr);

		printf("The file [%s] is not existed\n", file_name);
	//	exit(1);
	}

	//�ļ����ڣ���װ200 OK�������ļ�
	else
	{
	//��λƫ�����õ��ļ�����
	fseek(res_file,0,SEEK_END);
	file_len=ftell(res_file);
	fseek(res_file,0,SEEK_SET);

	//�ж��ļ�����
	type=http_suffix2type(suffix);
	//�ж��ļ������Ƿ����
	if(type==NULL)
	{
		printf("the type [%s] is not existed\n",type);
		exit(1);
	}

	//����http�ײ���
	hdr_len=sprintf(http_hdr,http_res_mes,file_len,type);
	send_len=send(socket,http_hdr,hdr_len,0);

	printf("%s\n",http_hdr);

	if (send_len == SOCKET_ERROR)
    {
        fclose(res_file);
        printf("send failed");
        exit(1);
    }

	//���Ͷ����ļ�
	do
	{
		read_len=fread(read_buf,sizeof(char),1024,res_file);
        if (read_len>0)
        {
            send_len=send(socket,read_buf,read_len,0);
            file_len-=read_len;
        }
	}while((read_len>0)&&(file_len>0));

	fclose(res_file);
	return 1;
}
}

//���������
int main()
{
	int len=0;
	WSADATA w;	//��ʼ������
	SOCKET server,client;	//�����׽���
	SOCKADDR_IN saddr,caddr;	//����������Ϳͻ�����Ϣ
	int result,recv_len;

	//��ʼ������,���ж��Ƿ�ɹ�
	if((WSAStartup(MAKEWORD(2,2),&w))!=0)
	{
		printf("winsock failed\n");
		exit(1);
	}
	else
		printf("winsock successed\n");

	//�����׽���,���ж��Ƿ�ɹ�
	server=socket(AF_INET,SOCK_STREAM,0);
	if(server==INVALID_SOCKET)
	{
		printf("server socket failed");
		exit(1);
	}
	else
		printf("server socket successed\n");

	//���÷�������Ϣ
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(80);
	saddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");

	//�׽�����������˿ڰ󶨣����ж��Ƿ�ɹ�
	if(bind(server,(SOCKADDR *)&saddr,sizeof(SOCKADDR))!=0)
	{
		printf("bind failed");
		exit(1);
	}
	else
		printf("bind successed\n");

	//�������ģʽ�����ж��Ƿ�ɹ�
	if(listen(server,5) == SOCKET_ERROR)
	{
		printf("listen failed");
		exit(1);
	}
	else
		printf("listen successed\n");

	len=sizeof(SOCKADDR);

	//����ѭ���ȴ��ͻ�������
	while(1)
	{
		char send_buf[1000]="",recv_buf[1000]="";
		//�ȴ��ͻ����ӣ��ӷ������׽��ֳ�������Ϣ�ɿͻ������׽��ֽ��գ��׽����൱����
		client=accept(server,(SOCKADDR *)&caddr,&len);
		//�ж������Ƿ�ɹ�
		if(client == INVALID_SOCKET)
		{
			printf("accept failed\n");
			exit(1);
		}
		else
		printf("accept successed\n");

		//���տͻ��˷�����Ϣ��������Ϣ����
		recv_len=recv(client,recv_buf,1000,0);
		//�жϽ����Ƿ�ɹ�
		if(recv_len == INVALID_SOCKET)
		{
			printf("recv failed\n");
			exit(1);
		}
		else
		printf("recv successed\n\n");
		printf("%s\n",recv_buf);
		//������Ϣ���ͻ���
		result=http_res(client,recv_buf,recv_len);
		//������ϵ���رտͻ����׽���
		closesocket(client);
	//	exit(1);
	}
	//�����˳�ѭ������ն�socket���ʹ�ã��رշ������׽���
	WSACleanup();
	closesocket(server);
	return 0��

}
