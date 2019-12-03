#include "Server_common.h"
#include "Server_func.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

Server_net::Server_net()
{
    mServer_command = new Server_command(this);
}

/*変数の宣言*/
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
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    /* ソケットを作成する */
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

    /* クライアントと接続する */
    maxfd = MultiAccept(request_soc, gClientNum);
    close(request_soc);

    if (maxfd == -1)
        return -1;

    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 20;

    /* 全クライアントの全ユーザー名を送る */
    SendAllName();

    /* select()のためのマスク値を設定する */
    SetMask(maxfd);

    return 0;
}

int Server_net::SendRecvManager()
{
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

                /* コマンドに対する処理を行う */
                endFlag = Server_command::ExecuteCommand(i);
            }
            if (endFlag == 0)
                break;
        }
    }

    return endFlag;
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
    }
}

void Server_net::Ending(void)
{
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
            /* スレッドの作成*/
            thr = SDL_CreateThread(NetworkEvent, "NetworkThread", &endFlag);
            Enter(i, fd);
        }
    }
    return fd;
}

void Server_net::Enter(int pos, int fd)
{

    printf("プレイヤーID%dさんが入室しました\n", pos);

    mClients[pos].fd = fd;
}

void Server_net::SetMask(int maxfd)
{
    int i;

    gWidth = maxfd + 1;

    FD_ZERO(&gMask);
    for (i = 0; i < gClientNum; i++)
        FD_SET(mClients[i].fd, &gMask);
}

void Server_net::SendAllName(void)
{
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

    assert(0 <= pos && pos < gClientNum);
    assert(data != NULL);
    assert(0 < dataSize);

    return read(mClients[pos].fd, data, dataSize);
}

/*****************************************************************
関数名  : NetworkEvent
機能    : ネットワークのイベントループ
引数    : void		*data		: 終了判定フラグ
出力    : スレッド終了時に０を返す
*****************************************************************/
int Server_net::NetworkEvent(void *data)
{
    int *endflag;
    running = true;
    Uint32 countTime;

    endflag = (int *)data;
    while (*endflag)
    {
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), countTime + 16))
            ;
        countTime = SDL_GetTicks();
        *endflag = SendRecvManager();
    }
    printf("スレッドの終了\n");
    running = false;
    return 0;
}