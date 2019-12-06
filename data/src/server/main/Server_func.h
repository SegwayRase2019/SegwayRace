#pragma once
#include"./Server_common.h"
#include <sys/time.h>

/* クライアントを表す構造体 */
typedef struct
{
  int fd;
  char name[MAX_NAME_SIZE];
} CLIENT;

class Server_net
{

public:
  Server_net();
  //^Server_net();                  /* デストラクタ */

  static CLIENT mClients[MAX_CLIENTS]; /* クライアント */

  static int gClientNum; /* クライアント数 */

  static struct timeval tv;

  static fd_set gMask; /* select()用のマスク */
  static int gWidth;   /* gMask中のチェックすべきビット数 */

  SDL_Thread *thr;
  static int endFlag;
  static bool running;

  static int NetworkEvent(void *data);
  int SetUpServer(int num);
  static int SendRecvManager(void);
  void Ending(void);
  static int RecvIntData(int pos, int *intData);
  static void SendData(int pos, void *data, int);
  static int RecvData(int pos, void *data, int dataSize);

private:
  class Server_command *mServer_command;
  int MultiAccept(int request_soc, int num);
  void Enter(int pos, int fd);
  void SetMask(int maxfd);
  void SendAllName(void);
};

class Server_command
{
public:
  Server_command(class Server_net *net);
  static int ExecuteCommand(int pos);
  int strage = 0;

private:
  class Server_net *mServer_net;
};

