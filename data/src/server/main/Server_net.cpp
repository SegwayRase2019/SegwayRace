<<<<<<< HEAD
#include "Game.h"
#include "Server_common.h"
#include"Server_func.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
=======

#include "Server_common.h"
#include "Server_func.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
>>>>>>> 98288e8... ネットワークのマルチスレッド化
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
=======
#include <sys/time.h>
>>>>>>> 98288e8... ネットワークのマルチスレッド化
#include <unistd.h>

Server_net::Server_net()
{
    mServer_command = new Server_command(this);
}

<<<<<<< HEAD
int Server_net::SetUpServer(int num)
{
    struct sockaddr_in	server;
    int			request_soc;
    int                 maxfd;
    int			val = 1;
 
    /* 引き数チェック */
    assert(0<num && num<=MAX_CLIENTS);

    gClientNum = num;
    bzero((char*)&server,sizeof(server));
=======
CLIENT Server_net::mClients[MAX_CLIENTS];
int Server_net::endFlag = 1;
int Server_net::gClientNum;
int Server_net::gWidth;
fd_set Server_net::gMask;
timeval Server_net::tv;
bool Server_net::running;

static int request_soc;

int Server_net::SetUpServer(int num)
{
    struct sockaddr_in server;

    int maxfd;
    int val = 1;

    /* 引き数チェック */
    assert(0 < num && num <= MAX_CLIENTS);

    gClientNum = num;
    bzero((char *)&server, sizeof(server));
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    /* ソケットを作成する */
<<<<<<< HEAD
    if((request_soc = socket(AF_INET,SOCK_STREAM,0)) < 0){
		fprintf(stderr,"Socket allocation failed\n");
		return -1;
    }
    setsockopt(request_soc,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));

    /* ソケットに名前をつける */
    if(bind(request_soc,(struct sockaddr*)&server,sizeof(server))==-1){
		fprintf(stderr,"Cannot bind\n");
		close(request_soc);
		return -1;
    }
    fprintf(stderr,"Successfully bind!\n");
    
    /* クライアントからの接続要求を待つ */
    if(listen(request_soc, gClientNum) == -1){
		fprintf(stderr,"Cannot listen\n");
		close(request_soc);
		return -1;
    }
    fprintf(stderr,"Listen OK\n");
=======
    if ((request_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Socket allocation failed\n");
        return -1;
    }
    setsockopt(request_soc, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    /* ソケットに名前をつける */
    if (bind(request_soc, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        fprintf(stderr, "Cannot bind\n");
        close(request_soc);
        return -1;
    }
    fprintf(stderr, "Successfully bind!\n");

    /* クライアントからの接続要求を待つ */
    if (listen(request_soc, gClientNum) == -1)
    {
        fprintf(stderr, "Cannot listen\n");
        close(request_soc);
        return -1;
    }
    fprintf(stderr, "Listen OK\n");
>>>>>>> 98288e8... ネットワークのマルチスレッド化

    /* クライアントと接続する */
    maxfd = MultiAccept(request_soc, gClientNum);
    close(request_soc);
<<<<<<< HEAD
    if(maxfd == -1)return -1;
=======

    if (maxfd == -1)
        return -1;

    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 20;
>>>>>>> 98288e8... ネットワークのマルチスレッド化

    /* 全クライアントの全ユーザー名を送る */
    SendAllName();

    /* select()のためのマスク値を設定する */
    SetMask(maxfd);

    return 0;
}

int Server_net::SendRecvManager()
{
<<<<<<< HEAD
    char	command;
    fd_set	readOK;
    int		i;
    int		endFlag = 1;

    readOK = gMask;
    /* クライアントからデータが届いているか調べる */
    if(select(gWidth,&readOK,NULL,NULL,NULL) < 0){
        /* エラーが起こった */
        return endFlag;
    }

    for(i=0;i<gClientNum;i++){
		if(FD_ISSET(mClients[i].fd,&readOK)){
	    	/* クライアントからデータが届いていた */
	    	/* コマンドを読み込む */
			RecvData(i,&command,sizeof(char));
	    	/* コマンドに対する処理を行う */
	    	endFlag = mServer_command->ExecuteCommand(command,i);
	    	if(endFlag == 0)break;
		}
=======
    char command;
    int i;
    int retval;
    fd_set readOK;

    readOK = gMask;
    /* クライアントからデータが届いているか調べる */
    retval = select(gWidth, &readOK, NULL, NULL, &tv);
    if (retval == -1)
    {
        /* エラーが起こった */
        return endFlag;
    }
    else if (retval)
    {

        for (i = 0; i < gClientNum; i++)
        {

            if (FD_ISSET(mClients[i].fd, &readOK))
            {
                /* クライアントからデータが届いていた */
                /* コマンドを読み込む */
                RecvData(i, &command, sizeof(char));
                /* コマンドに対する処理を行う */
                endFlag = Server_command::ExecuteCommand(command, i);
                if (endFlag == 0)
                    break;
            }
        }
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    }
    return endFlag;
}

<<<<<<< HEAD
int Server_net::RecvIntData(int pos,int *intData)
{
    int n,tmp;
    
    /* 引き数チェック */
    assert(0<=pos && pos<gClientNum);
    assert(intData!=NULL);

    n = RecvData(pos,&tmp,sizeof(int));
    (*intData) = ntohl(tmp);
    
    return n;
}

void Server_net::SendData(int pos,void *data,int dataSize)
{
    int	i;
   
    /* 引き数チェック */
    assert(0<=pos && pos<gClientNum || pos==ALL_CLIENTS);
    assert(data!=NULL);
    assert(0<dataSize);

    if(pos == ALL_CLIENTS){
    	/* 全クライアントにデータを送る */
		for(i=0;i<gClientNum;i++){
			write(mClients[i].fd,data,dataSize);
		}
    }
    else{
		write(mClients[pos].fd,data,dataSize);
=======
int Server_net::RecvIntData(int pos, int *intData)
{
    int n, tmp;

    /* 引き数チェック */
    assert(0 <= pos && pos < gClientNum);
    assert(intData != NULL);

    n = RecvData(pos, &tmp, sizeof(int));
    (*intData) = ntohl(tmp);

    return n;
}

void Server_net::SendData(int pos, void *data, int dataSize)
{
    int i;

    /* 引き数チェック */
    assert(0 <= pos && pos < gClientNum || pos == ALL_CLIENTS);
    assert(data != NULL);
    assert(0 < dataSize);

    if (pos == ALL_CLIENTS)
    {
        /* 全クライアントにデータを送る */
        for (i = 0; i < gClientNum; i++)
        {
            write(mClients[i].fd, data, dataSize);
        }
    }
    else
    {
        write(mClients[pos].fd, data, dataSize);
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    }
}

void Server_net::Ending(void)
{
<<<<<<< HEAD
    int	i;

    printf("... Connection closed\n");
    for(i=0;i<gClientNum;i++)close(mClients[i].fd);
}

int Server_net::MultiAccept(int request_soc,int num)
{
    int	i,j;
    int	fd;
    
    for(i=0;i<num;i++){
		if((fd = accept(request_soc,NULL,NULL)) == -1){
			fprintf(stderr,"Accept error\n");
			for(j=i-1;j>=0;j--)close(mClients[j].fd);
			return -1;
		}
		Enter(i,fd);
=======
    int i;

    SDL_WaitThread(thr, NULL);

    printf("... Connection closed\n");
    for (i = 0; i < gClientNum; i++)
        close(mClients[i].fd);
}

int Server_net::MultiAccept(int request_soc, int num)
{
    int i, j;
    int fd;

    for (i = 0; i < num; i++)
    {
        if ((fd = accept(request_soc, NULL, NULL)) == -1)
        {
            fprintf(stderr, "Accept error\n");
            for (j = i - 1; j >= 0; j--)
                close(mClients[j].fd);
            return -1;
        }
        else
        {
            thr = SDL_CreateThread(NetworkEvent, "NetworkThread", &endFlag);
            Enter(i, fd);
        }
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    }
    return fd;
}

void Server_net::Enter(int pos, int fd)
{
<<<<<<< HEAD
	/* クライアントのユーザー名を受信する */
	read(fd,mClients[pos].name,MAX_NAME_SIZE);
#ifndef NDEBUG
	printf("%s is accepted\n",mClients[pos].name);
#endif
	mClients[pos].fd = fd;
=======
    /* クライアントのユーザー名を受信する */
    read(fd, mClients[pos].name, MAX_NAME_SIZE);
#ifndef NDEBUG
    printf("%s is accepted\n", mClients[pos].name);
#endif
    mClients[pos].fd = fd;
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

void Server_net::SetMask(int maxfd)
{
<<<<<<< HEAD
    int	i;

    gWidth = maxfd+1;

    FD_ZERO(&gMask);    
    for(i=0;i<gClientNum;i++)FD_SET(mClients[i].fd,&gMask);
=======
    int i;

    gWidth = maxfd + 1;

    FD_ZERO(&gMask);
    for (i = 0; i < gClientNum; i++)
        FD_SET(mClients[i].fd, &gMask);
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

void Server_net::SendAllName(void)
{
<<<<<<< HEAD
  int	i,j,tmp1,tmp2;

    tmp2 = htonl(gClientNum);
    for(i=0;i<gClientNum;i++){
		tmp1 = htonl(i);
		SendData(i,&tmp1,sizeof(int));
		SendData(i,&tmp2,sizeof(int));
		for(j=0;j<gClientNum;j++){
			SendData(i,mClients[j].name,MAX_NAME_SIZE);
		}
	}
}

int Server_net::RecvData(int pos,void *data,int dataSize)
{
    int n;
    
    /* 引き数チェック */
    assert(0<=pos && pos<gClientNum);
    assert(data!=NULL);
    assert(0<dataSize);

    n = read(mClients[pos].fd,data,dataSize);
    
    return n;
}
=======
    int i, j, tmp1, tmp2;

    tmp2 = htonl(gClientNum);
    for (i = 0; i < gClientNum; i++)
    {
        tmp1 = htonl(i);
        SendData(i, &tmp1, sizeof(int));
        SendData(i, &tmp2, sizeof(int));
        for (j = 0; j < gClientNum; j++)
        {
            SendData(i, mClients[j].name, MAX_NAME_SIZE);
        }
    }
}

int Server_net::RecvData(int pos, void *data, int dataSize)
{
    int n;

    /* 引き数チェック */
    assert(0 <= pos && pos < gClientNum);
    assert(data != NULL);
    assert(0 < dataSize);

    n = read(mClients[pos].fd, data, dataSize);

    return n;
}

int Server_net::NetworkEvent(void *data)
{
    int *endflag;
    running = true;

    endflag = (int *)data;
    while (*endflag)
    {
        *endflag = SendRecvManager();
    }
    printf("スレッドの終了\n");
    running = false;
    return 0;
}
>>>>>>> 98288e8... ネットワークのマルチスレッド化
