#pragma once
#include "Server_func.h"
#include "Collision.h"
#include <cmath>
#include <stdlib.h>

class Calculate
{
public:
  Calculate();

  static int Player_restitution(CONTAINER Posdata); //反発係数の計算

  static int Stage_rank(CONTAINER Posdata); //順位の計算

  static int Calculate_angle();
  static float Player_angle[MAX_CLIENTS];
  static float Player_angle_copy[MAX_CLIENTS];
  static float Player_laps[MAX_CLIENTS];

  static bool Warnig_mode;
  static float Before_degree[MAX_CLIENTS];
  static float CollisionStrage[MAX_CLIENTS];

  static float v2;
  static float m2;

  static float SpeedStorage[MAX_CLIENTS];

private:
#define SIZE_OF_ANGLE(Player_angle) (sizeof(Player_angle) / sizeof(Player_angle[0]))
};
