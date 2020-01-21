#pragma once
#include "../../common/Common.h"
#include "./Client_func.h"
#include "./Client_net.h"
#include "../ui/Canvas.h"
#include "../ui/Font.h"
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "../Wiifit/WiiFit_common.h"

// Game class
class Game
{
public:
  enum GameState
  {
    ERunning,
    EFinished
  };
  Game();
  // Initialize the game
  bool Initialize(int, char *[]);
  // Runs the game loop until the game is over
  void RunLoop();
  // Shutdown the game
  void Shutdown();

  // Create Thread
  bool CreateTread(SDL_ThreadFunction fc, const char *name);

  void AddActor(class Actor *actor);
  void RemoveActor(class Actor *actor);

  void AddSprite(class SpriteComponent *sprite);
  void RemoveSprite(class SpriteComponent *sprite);

  const std::vector<class Canvas *> &GetUIStack() { return mUIStack; }
  void PushUI(class Canvas *canvas);

  class Font *GetFont(const std::string &fileName);

  void LoadText(const std::string &fileName);
  const std::string &GetText(const std::string &key);

  static int clientID;
  static CONTAINER Player_difference[MAX_CLIENTS];
  static CONTAINER Collision_difference[MAX_CLIENTS];

  // Getter
  class Client_net *GetClient_net(void) const { return mNet; }
  class Client_window *GetClient_window(void) const { return mWindow; }
  class Client_command *GetClient_command(void) const { return mCommand; }
  class HUD *GetHUD(void) const { return mHUD; }
  int GetClientNum(void) const { return mNum; }
  int GetEndFlag(void) const { return mEndFlag; }

  class Racer *mRacer[MAX_CLIENTS];
  class Player *mPlayer;
  class ItemBox *mItem;

  //wiifitを定義
  static int Wiifit_Thread(void *data);
  static Prs prs;

  struct balance_cal balance_cal;
  static char command; //転送するコマンド
  bool wiifit_connect = true;

  static int volume;

private:
  class Client_net *mNet;
  class Client_window *mWindow;
  class Client_command *mCommand;
  class HUD *mHUD;

  // network thread
  SDL_Thread *thr;
  //wiifit thread
  SDL_Thread *wii_fit_thr;

  // Helper functions for the game loop
  void ProcessInput();
  // Update
  void UpdateGame();
  //Draw Renderer
  void GenerateOutput();
  void LoadData();
  void UnloadData();

  static int NetworkEvent(void *data);

  static void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                             union cwiid_mesg mesg[], struct timespec *ts);

  // All the actors in the game
  std::vector<class Actor *> mActors;
  // Any pending actors
  std::vector<class Actor *> mPendingActors;

  std::vector<class Canvas *> mUIStack;
  std::unordered_map<std::string, class Font *> mFonts;

  std::unordered_map<std::string, std::string> mText;

  //*static*/ int NetworkEvent(void *data);

  // Number of ticks since start of game
  Uint32 mTicksCount;
  // Game should continue to run
  bool mIsRunning;
  int mEndFlag;
  int mWiiFlag;

  // Track if we're updating actors right now
  bool mUpdatingActors;

  float mIntervalTime;
  float mCountTimer;

  GameState mGameState;

  int mNum;
};
