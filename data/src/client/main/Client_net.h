#pragma once

#include "./Game.h"
#include "./Client_func.h"
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

class Client_net
{
public:
  Client_net();
  Client_net(class Game *game);

  int SetUpClient(char *, int *, int *, char clientName[][MAX_NAME_SIZE]);
  static int SendRecvManager(void);
  int RecvIntData(int *);
  void SendData(void *data, int dataSize);
  void CloseSoc(void);
  static int RecvData(void *, int);

private:
  class Game *mGame;
  class Client_command *mClient_command;
  void GetAllName(int *, int *, char clientNames[][MAX_NAME_SIZE]);
  void SetMask(void);
};