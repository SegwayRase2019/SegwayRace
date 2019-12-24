#pragma once
#include "Server_func.h"
#include <cmath>
#include <stdlib.h>
#include "./Server_calculate.h"

class Collision
{
public:
  Collision();

  static int Player_Collision(CONTAINER Posdata);
  static int Stage_Collision(CONTAINER Posdata);

  static int Collision_Judgement(CONTAINER Posdata);

  static int Collision_item(CONTAINER Posdata);

  static bool collision;
  static float distance;

  static int collision_oppnent;
  static Vector2 Collision_Vector[MAX_CLIENTS];
  static Vector2 Item_coordinate;

  static bool Item_effect;

  static CONTAINER PlayerPos[MAX_CLIENTS];
  static int Player_Collision_Strage[MAX_CLIENTS];
  static float Player_Speed[MAX_CLIENTS];
  static int Collisioned_opponent[MAX_CLIENTS];

private:
};