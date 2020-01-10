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
  void SendStartSignal(void);

  static bool isStart;
  static bool isGoal[MAX_CLIENTS];
  static float Back_speed;
  static int Collisioned_oppnent;
  static bool Oppnent;
  static Vector2 CollisionVector[MAX_CLIENTS];
  static bool isRepulsion;
  static bool isCollision;
  static CONTAINER CollisionPos[MAX_CLIENTS];

private:
  class Game *mGame;
  class Client_window *mWindow;
  class Client_net *mClient_net;
  void RecvCircleData(void);
  void RecvRectangleData(void);
  void RecvDiamondData(void);
  int strage; /*あとで消す*/
};
