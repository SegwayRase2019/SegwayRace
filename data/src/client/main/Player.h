#pragma once

#include "Actor.h"

class Player : public Actor
{
  public:
    Player(class Game *game);
    //playerの移動
    void UpdateActor(float deltaTime) override;
    //キーボードからの入力を読み込む
    void Input_Keybord(float unit8_t *state);
    //wiiboardからの入力
    void Input_Wiiboard();

  private:
    int pnum;
    float speed;
    float weight;
};