#pragma once
#include"./Server_common.h"


class Server_net
{
public:
    Server_net();
    //^Server_net();                  /* デストラクタ */

    int	gClientNum;					/* クライアント数 */

    fd_set	gMask;					/* select()用のマスク */
    int	gWidth;						/* gMask中のチェックすべきビット数 */

    int SetUpServer(int num);
    int SendRecvManager(void);
    void Ending(void);
    int RecvIntData(int pos,int *intData);
    void SendData(int pos, void *data,int dataSize);

private:
    /* クライアントを表す構造体 */
    typedef struct{
	    int		fd;
	    char	name[MAX_NAME_SIZE];
    }CLIENT;

    CLIENT	mClients[MAX_CLIENTS];	/* クライアント */
    
    class Server_command *mServer_command;
    int MultiAccept(int request_soc,int num);
    void Enter(int pos,int fd);
    void SetMask(int maxfd);
    void SendAllName(void);
    int RecvData(int pos, void *data,int dataSize);
};

class Server_command{
    public:
    Server_command(class Server_net *net);
    int ExecuteCommand(char command,int pos);
    void SendDiamondCommand(void);
    private:


    void SetIntData2DataBlock(void *,int,int *);
    void SetCharData2DataBlock(void * ,char, int *);
    int GetRandomInt(int);

private:
    class Server_net *mServer_net;
};
