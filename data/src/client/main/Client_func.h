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

  static int ExecuteCommand();
  static CONTAINER PlayerPos[MAX_CLIENTS];
  void SendEndCommand(void);
  void SendPosCommand(void);
  void SendStartSignal(void);

  static bool isCollision;
  static bool isStart;

private:
  class Game *mGame;
  class Client_window *mWindow;
  class Client_net *mClient_net;
  void RecvCircleData(void);
  void RecvRectangleData(void);
  void RecvDiamondData(void);
  int strage; /*あとで消す*/
};
