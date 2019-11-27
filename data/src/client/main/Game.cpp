#include "Game.h"

Game::Game()
    : mEndFlag(1)
{
}

bool Game::Initialize(int argc, char *argv[])
{
    char name[MAX_CLIENTS][MAX_NAME_SIZE];
    char localHostName[] = "localhost";
    char *serverName;
    int clientID;
    class Client_command *command;

    mNet = new Client_net(this);
    mWindow = new Client_window(this);
    mCommand = new Client_command(this);

    /* 引き数チェック */
    if (argc == 1)
    {
        serverName = localHostName;
    }
    else if (argc == 2)
    {
        serverName = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
        return -1;
    }

    /* サーバーとの接続 */
    if (mNet->SetUpClient(serverName, &clientID, &mNum, name) == -1)
    {
        fprintf(stderr, "setup failed : SetUpClient\n");
        return -1;
    }
    /* ウインドウの初期化 */
    if (!mWindow->InitWindows(clientID, mNum, name))
    {
        fprintf(stderr, "setup failed : InitWindows\n");
        return -1;
    }

    /* ネットワークイベント処理スレッドの作成 */
    thr = SDL_CreateThread(NetworkEvent, "NetworkThread", &mEndFlag);

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop()
{
    ProcessInput();
    UpdateGame();
    GenerateOutput();
}

void Game::Shutdown()
{
    /* 終了処理 */
    SDL_WaitThread(thr, NULL);
    mWindow->DestroyWindow();
    mNet->CloseSoc();

    SDL_Quit();
}

//private

void Game::ProcessInput()
{
    // 試験的な実装
    mWindow->WindowEvent(mNum);

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    //if escape is pressed, also end loop
    if (state[SDL_SCANCODE_UP])
    {
        mCommand->SendPosCommand();
    }
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();
}

void Game::GenerateOutput()
{
}

/*****************************************************************
関数名  : NetworkEvent
機能    : ネットワークのイベントループ
引数    : void		*data		: 終了判定フラグ
出力    : スレッド終了時に０を返す
*****************************************************************/
int Game::NetworkEvent(void *data)
{
    int *endFlag;

    endFlag = (int *)data;
    while (*endFlag)
    {
        *endFlag = Client_net::SendRecvManager();
    }
    return 0;
}