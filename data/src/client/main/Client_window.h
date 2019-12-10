#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "./Game.h"

class Client_window
{
public:
  Client_window(class Game *game);
  ~Client_window();

  bool InitWindows(int clientID, int num, char name[][MAX_NAME_SIZE]);
  void DestroyWindow(void);
  void UnloadData();

  void AddSprite(class SpriteComponent *sprite);
  void RemoveSprite(class SpriteComponent *sprite);

  void Draw();

  SDL_Texture *GetTexture(const std::string &filename);

  // getter
  int GetWidth() const { return mWindowWidth; }
  int GetHeight() const { return mWindowHeight; }

private:
  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  class Game *mGame;
  class Client_command *mClient_command;

  int mWindowWidth;
  int mWindowHeight;

  // All the sprite components drawn
  std::vector<class SpriteComponent *> mSprites;

  std::unordered_map<std::string, SDL_Texture *> mTextures;
};