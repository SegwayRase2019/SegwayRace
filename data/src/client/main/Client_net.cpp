#include "Client_func.h"
#include "./Client_net.h"
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 100

static int gSocket;  /* ソケット */
static fd_set gMask; /* select()用のマスク */
static int gWidth;   /* gMask中ののチェックすべきビット数 */

Client_net::Client_net(Game *game)
{
    mGame = game;
}

Client_net::Client_net()
{
}

int Client_net::SetUpClient(char *hostName, int *clientID, int *num, char clientNames[][MAX_NAME_SIZE])
{
    mClient_command = mGame->getClient_command();

    struct hostent *servHost;
    struct sockaddr_in server;
    int len;
    char str[BUF_SIZE];

    /* ホスト名からホスト情報を得る */
    if ((servHost = gethostbyname(hostName)) == NULL)
    {
        fprintf(stderr, "Unknown host\n");
        return -1;
    }

    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    bcopy(servHost->h_addr, (char *)&server.sin_addr, servHost->h_length);

    /* ソケットを作成する */
    if ((gSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket allocation failed\n");
        return -1;
    }

    /* サーバーと接続する */
    if (connect(gSocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        fprintf(stderr, "cannot connect\n");
        close(gSocket);
        return -1;
    }
    fprintf(stderr, "connected\n");

    /* 名前を読み込みサーバーに送る */

    do
    {
        printf("Enter Your Name\n");
        fgets(str, BUF_SIZE, stdin);
        len = strlen(str) - 1;
        str[len] = '\0';
    } while (len > MAX_NAME_SIZE - 1 || len == 0);
    SendData(str, MAX_NAME_SIZE);

    printf("Please Wait\n");

    /* 全クライアントのユーザー名を得る */
    GetAllName(clientID, num, clientNames);

    /* select()のためのマスク値を設定する */
    SetMask();

    return 0;
}

int Client_net::SendRecvManager(void)
{
    fd_set readOK;
    char command;
    int i;
    int endFlag = 1;
    struct timeval timeout;

    CONTAINER Posdata;

    memset(&Posdata, 0, sizeof(CONTAINER));

    /* select()の待ち時間を設定する */
    timeout.tv_sec = 0;
    timeout.tv_usec = 20;

    readOK = gMask;
    /* サーバーからデータが届いているか調べる */
    select(gWidth, &readOK, NULL, NULL, &timeout);
    if (FD_ISSET(gSocket, &readOK))
    {
        /* サーバーからデータが届いていた */
        /* コマンドを読み込む */
        /* コマンドに対する処理を行う */
        endFlag = Client_command::ExecuteCommand();
    }
    return endFlag;
}

int Client_net::RecvIntData(int *intData)
{
    int n, tmp;

    /* 引き数チェック */
    assert(intData != NULL);

    n = RecvData(&tmp, sizeof(int));
    (*intData) = ntohl(tmp);

    return n;
}

void Client_net::SendData(void *data, int dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 < dataSize);

    write(gSocket, data, dataSize);
}

void Client_net::CloseSoc(void)
{
    printf("...Connection closed\n");
    close(gSocket);
}

void Client_net::GetAllName(int *clientID, int *num, char clientNames[][MAX_NAME_SIZE])
{
    int i;

    /* クライアント番号の読み込み */
    RecvIntData(clientID);
    /* クライアント数の読み込み */
    RecvIntData(num);

    /* 全クライアントのユーザー名を読み込む */
    for (i = 0; i < (*num); i++)
    {
        RecvData(clientNames[i], MAX_NAME_SIZE);
    }
#ifndef NDEBUG
    printf("#####\n");
    printf("client number = %d\n", (*num));
    for (i = 0; i < (*num); i++)
    {
        printf("%d:%s\n", i, clientNames[i]);
    }
#endif
}

void Client_net::SetMask(void)
{
    int i;

    FD_ZERO(&gMask);
    FD_SET(gSocket, &gMask);

    gWidth = gSocket + 1;
}

int Client_net::RecvData(void *data, int dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 < dataSize);

    return read(gSocket, data, dataSize);
}
