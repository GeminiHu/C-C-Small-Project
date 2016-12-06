#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib");

struct doc_type
{
	char *suffix;
	char *type;
};
//定义的可供查询的文件类型
struct doc_type file_type[]=
{
	{"html","html"},
	{"jpg","jpg"},
	{"txt","txt"},
	{"ico","ico"},
	{NULL,NULL}
};

//成功找到，200 OK响应报文
char *http_res_mes="HTTP/1.1 200 OK\r\n"//404 NOT FOUND
	"Accept-Ranges: bytes\r\n"
	"Content-Length: %d\r\n"
	"Connection: close\r\n"
	"Content-type: %s\r\n\r\n";

/***************************************
*
*函数功能：根据文件后缀查找对应的Content-Type
*输入suffix为对象文件后缀名
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
*函数功能：解析HTTP报文请求行，得到文件名和后缀名
*buf为客户端返回服务器的信息
*file_name为对象文件名
*suffix为对象文件后缀名
****************************************/
void http_request(char *buf,int buflen,char *file_name,char *suffix)
{
    int length=0;
	//开始点、结束点、偏移点
    char *begin,*end,*bias;

	//请求行的特点，两个空格之间为URL

    //查找URL的开始位置
    begin=strchr(buf,' ');
    begin+=1;

    //查找 URL 的结束位置
    end=strchr(begin,' ');
    *end=0;

    bias=strrchr(begin,'/');
    length=end-bias;

    //找到文件名的开始位置
    if((*bias=='/')||(*bias=='\\'))
    {
        bias++;
        length--;
    }

    // 得到文件名
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
*函数功能：服务器向客户端发送HTTP响应
*socket为客户端的套接字
*buf为客户端发送的请求报文信息
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

	//得到文件名后缀
	http_request(buf,buf_len,file_name,suffix);
	res_file=fopen(file_name,"rb+");

	//文件不存在，404报文
	if(res_file==NULL)
	{
		char *http_res_mes="HTTP/1.1 404 Not Found\r\n";
		hdr_len=sprintf(http_hdr,http_res_mes);
		send_len=send(socket,http_hdr,hdr_len,0);
		printf("%s\n",http_hdr);

		printf("The file [%s] is not existed\n", file_name);
	//	exit(1);
	}

	//文件存在，封装200 OK和请求文件
	else
	{
	//定位偏移量得到文件长度
	fseek(res_file,0,SEEK_END);
	file_len=ftell(res_file);
	fseek(res_file,0,SEEK_SET);

	//判断文件类型
	type=http_suffix2type(suffix);
	//判断文件类型是否存在
	if(type==NULL)
	{
		printf("the type [%s] is not existed\n",type);
		exit(1);
	}

	//构造http首部行
	hdr_len=sprintf(http_hdr,http_res_mes,file_len,type);
	send_len=send(socket,http_hdr,hdr_len,0);

	printf("%s\n",http_hdr);

	if (send_len == SOCKET_ERROR)
    {
        fclose(res_file);
        printf("send failed");
        exit(1);
    }

	//发送对象文件
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

//主函数入口
int main()
{
	int len=0;
	WSADATA w;	//初始化操作
	SOCKET server,client;	//定义套接字
	SOCKADDR_IN saddr,caddr;	//定义服务器和客户端信息
	int result,recv_len;

	//初始化操作,并判断是否成功
	if((WSAStartup(MAKEWORD(2,2),&w))!=0)
	{
		printf("winsock failed\n");
		exit(1);
	}
	else
		printf("winsock successed\n");

	//创建套接字,并判断是否成功
	server=socket(AF_INET,SOCK_STREAM,0);
	if(server==INVALID_SOCKET)
	{
		printf("server socket failed");
		exit(1);
	}
	else
		printf("server socket successed\n");

	//设置服务器信息
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(80);
	saddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");

	//套接字与服务器端口绑定，并判断是否成功
	if(bind(server,(SOCKADDR *)&saddr,sizeof(SOCKADDR))!=0)
	{
		printf("bind failed");
		exit(1);
	}
	else
		printf("bind successed\n");

	//进入监听模式，并判断是否成功
	if(listen(server,5) == SOCKET_ERROR)
	{
		printf("listen failed");
		exit(1);
	}
	else
		printf("listen successed\n");

	len=sizeof(SOCKADDR);

	//无限循环等待客户端连接
	while(1)
	{
		char send_buf[1000]="",recv_buf[1000]="";
		//等待客户连接，从服务器套接字出来的信息由客户机的套接字接收，套接字相当于门
		client=accept(server,(SOCKADDR *)&caddr,&len);
		//判断连接是否成功
		if(client == INVALID_SOCKET)
		{
			printf("accept failed\n");
			exit(1);
		}
		else
		printf("accept successed\n");

		//接收客户端返回信息，返回信息长度
		recv_len=recv(client,recv_buf,1000,0);
		//判断接收是否成功
		if(recv_len == INVALID_SOCKET)
		{
			printf("recv failed\n");
			exit(1);
		}
		else
		printf("recv successed\n\n");
		printf("%s\n",recv_buf);
		//发送信息到客户端
		result=http_res(client,recv_buf,recv_len);
		//结束联系，关闭客户机套接字
		closesocket(client);
	//	exit(1);
	}
	//如若退出循环，清空对socket库的使用，关闭服务器套接字
	WSACleanup();
	closesocket(server);
	return 0；

}
