#pragma once
#include "../../common/Common.h"
#include "./Game.h"
#include <unordered_map>
#include <string>
#include <vector>

class Client_command
{
  public:
    Client_command(class Game *game);

    int ExecuteCommand(char);
    void SendRectangleCommand(void);
    void SendCircleCommand(int pos);
    void SendEndCommand(void);

  private:
    class Game *mGame;
    class Client_net *mClient_net;
    class Client_window *mWindow;
    void SetIntData2DataBlock(void *, int, int *);
    void SetCharData2DataBlock(void *, char, int *);
    void RecvCircleData(void);
    void RecvRectangleData(void);
    void RecvDiamondData(void);
};
