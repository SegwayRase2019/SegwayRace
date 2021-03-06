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
  static CONTAINER PlayerPosCopy[MAX_CLIENTS];
  static float Player_weight[MAX_CLIENTS];
  void SendEndCommand(void);
  void SendPosCommand(void);
  void SendWaitSignal(void);
  void SendStartSignal(void);
  void SendPosCommand_i(void);

  static bool isPreparing;
  static bool isStart;
  static bool isGoal[MAX_CLIENTS];
  static bool isFinish;
  static float Back_speed;
  static int Collisioned_oppnent;
  static bool Oppnent;
  static Vector2 CollisionVector[MAX_CLIENTS];
  static bool isRepulsion;
  static bool isCollision;
  static CONTAINER CollisionPos[MAX_CLIENTS];
  static bool item_collision;
  static bool item_exist[ITEM];

private:
  class Game *mGame;
  class Client_window *mWindow;
  class Client_net *mClient_net;
  void RecvCircleData(void);
  void RecvRectangleData(void);
  void RecvDiamondData(void);
  int strage; /*あとで消す*/
};
