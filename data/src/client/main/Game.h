#pragma once
#include "../../common/Common.h"
<<<<<<< HEAD
#include "./Client_func.h"
=======
#include "Client_func.h"
>>>>>>> 98288e8... ネットワークのマルチスレッド化
#include "./Client_net.h"
#include <SDL2/SDL_ttf.h>
#include <vector>

// Game class
class Game
{
  public:
    Game();
    // Initialize the game
    bool Initialize(int, char *[]);
    // Runs the game loop until the game is over
    void RunLoop();
    // Shutdown the game
    void Shutdown();

    // Create Thread
    bool CreateTread(SDL_ThreadFunction fc, const char *name);

<<<<<<< HEAD
    void AddActor(class Actor *actor);
    void RemoveActor(class Actor *actor);

    void AddSprite(class SpriteComponent *sprite);
    void RemoveSprite(class SpriteComponent *sprite);

    // Getter
    class Client_net *GetClient_net(void) const { return mNet; }
    class Client_window *GetClient_window(void) const { return mWindow; }
    class Client_command *GetClient_command(void) const { return mCommand; }
    int GetEndFlag(void) { return mEndFlag; }

  private:
    class Client_net *mNet;
    class Client_window *mWindow;
    class Client_command *mCommand;
=======
    // getter
    class Client_net *getClient_net(void) const { return mNet; }
    class Client_window *getClient_window(void) const { return mWindow; }
    class Client_command *getClient_command(void) const { return mCommand; }
    int getEndFlag(void) { return mEndFlag; }

  private:
    class Client_net *mNet;
    class Client_command *mCommand;
    class Client_window *mWindow;
>>>>>>> 98288e8... ネットワークのマルチスレッド化

    // network thread
    SDL_Thread *thr;

    // Helper functions for the game loop
    void ProcessInput();
    // Update
    void UpdateGame();
    //Draw Renderer
    void GenerateOutput();

<<<<<<< HEAD
    int NetworkEvent(void *data);

    // All the actors in the game
    std::vector<class Actor *> mActors;
    // Any pending actors
    std::vector<class Actor *> mPendingActors;

=======
    static int NetworkEvent(void *data);

    //
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    //*static*/ int NetworkEvent(void *data);

    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    int mEndFlag;
<<<<<<< HEAD

    // Track if we're updating actors right now
    bool mUpdatingActors;

=======
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    // 以下は必要ない変数
    int mNum;
};
