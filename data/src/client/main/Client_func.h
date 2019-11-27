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
  void SendRectangleCommand(void);
  void SendCircleCommand(int pos);
  void SendEndCommand(void);
  void SendPosCommand(void);

private:
  class Game *mGame;
  class Client_window *mWindow;
  class Client_net *mClient_net;
  void SetIntData2DataBlock(void *, int, int *);
  void SetCharData2DataBlock(void *, char, int *);
  void RecvCircleData(void);
  void RecvRectangleData(void);
  void RecvDiamondData(void);
};

class Client_window
{
public:
  Client_window(class Game *game);

  bool InitWindows(int, int, char[][MAX_NAME_SIZE]);
  void DestroyWindow(void);
  void WindowEvent(int);
  void DrawRectangle(int, int, int, int);
  void DrawCircle(int, int, int);
  void DrawDiamond(int, int, int);

  SDL_Texture *getTexture(const std::string &filename);

private:
  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  class Game *mGame;
  class Client_command *mClient_command;

  std::unordered_map<std::string, SDL_Texture *> mTextures;

  int CheckButtonNO(int, int, int);
};