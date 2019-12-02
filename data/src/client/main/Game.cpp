#include "Game.h"
<<<<<<< HEAD
#include "./Client_window.h"
#include "../actor/Racer.h"
#include "../actor/Stage.h"
#include <algorithm>

Game::Game()
	: mEndFlag(1), mUpdatingActors(false)
=======

Game::Game()
    : mEndFlag(1)
>>>>>>> 98288e8... ネットワークのマルチスレッド化
{
}

bool Game::Initialize(int argc, char *argv[])
{
<<<<<<< HEAD
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
		fprintf(stderr, "Setup failed : SetUpClient\n");
		return -1;
	}
	/* ウインドウの初期化 */
	if (!mWindow->InitWindows(clientID, mNum, name))
	{
		fprintf(stderr, "Setup failed : InitWindows\n");
		return -1;
	}

	/* ネットワークイベント処理スレッドの作成 */
	//thr = SDL_CreateThread(Game::NetworkEvent,"NetworkThread" ,&mEndFlag);

	mTicksCount = SDL_GetTicks();

	class Racer *player = new Racer(this);
	class Stage *stage = new Stage(this);

	return true;
=======
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
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

void Game::RunLoop()
{
<<<<<<< HEAD
	ProcessInput();
	UpdateGame();
	GenerateOutput();
=======
    ProcessInput();
    UpdateGame();
    GenerateOutput();
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

void Game::Shutdown()
{
<<<<<<< HEAD
	/* 終了処理 */
	SDL_WaitThread(thr, NULL);
	mWindow->DestroyWindow();
	mNet->CloseSoc();

	SDL_Quit();
=======
    /* 終了処理 */
    SDL_WaitThread(thr, NULL);
    mWindow->DestroyWindow();
    mNet->CloseSoc();

    SDL_Quit();
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

//private

void Game::ProcessInput()
{
<<<<<<< HEAD
	NetworkEvent(&mEndFlag);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mEndFlag = false;
			break;
		}
	}

	const Uint8 *keyState = SDL_GetKeyboardState(NULL);

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
=======
    // 試験的な実装
    mWindow->WindowEvent(mNum);

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    //if escape is pressed, also end loop
    if (state[SDL_SCANCODE_UP])
    {
        mCommand->SendPosCommand();
    }
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

void Game::UpdateGame()
{
<<<<<<< HEAD
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor *> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::AddActor(Actor *actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor *actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent *sprite)
{
	mWindow->AddSprite(sprite);
}

void Game::RemoveSprite(SpriteComponent *sprite)
{
	mWindow->RemoveSprite(sprite);
=======
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

void Game::GenerateOutput()
{
<<<<<<< HEAD
	mWindow->Draw();
=======
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}

/*****************************************************************
関数名  : NetworkEvent
機能    : ネットワークのイベントループ
引数    : void		*data		: 終了判定フラグ
出力    : スレッド終了時に０を返す
*****************************************************************/
int Game::NetworkEvent(void *data)
{
<<<<<<< HEAD
	int *endFlag;

	endFlag = (int *)data;
	//while(*endFlag){
	*endFlag = mNet->SendRecvManager();
	//}
	return 0;
=======
    int *endFlag;

    endFlag = (int *)data;
    while (*endFlag)
    {
        *endFlag = Client_net::SendRecvManager();
    }
    return 0;
>>>>>>> 98288e8... ネットワークのマルチスレッド化
}