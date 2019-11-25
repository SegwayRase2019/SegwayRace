#pragma once
#include "../../common/Common.h"
#include "Client_func.h"
#include "./Client_net.h"
#include <SDL2/SDL_ttf.h>
#include <vector>

// Game class
class Game {
public:
    Game();
    // Initialize the game
    bool Initialize(int, char* []);
    // Runs the game loop until the game is over
    void RunLoop();
    // Shutdown the game
    void Shutdown();

    // Create Thread
    bool CreateTread(SDL_ThreadFunction fc, const char* name);

    // getter
    class Client_net* getClient_net(void)const{ return mNet; }
    class Client_window* getClient_window(void)const{ return mWindow; }
    class Client_command* getClient_command(void)const{ return mCommand; }
    int getEndFlag(void){ return mEndFlag; }

private:
    class Client_net *mNet;
    class Client_command *mCommand;
    class Client_window *mWindow;

    // network thread
	SDL_Thread	*thr;
    
    // Helper functions for the game loop
    void ProcessInput();
    // Update
    void UpdateGame();
    //Draw Renderer
    void GenerateOutput();

    int NetworkEvent(void* data);

    //
    //*static*/ int NetworkEvent(void *data);

    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    int mEndFlag;
    // 以下は必要ない変数
    int mNum;
};
