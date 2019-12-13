#include "Game.h"
#include "./Client_window.h"
#include "../actor/Racer.h"
#include "../actor/Player.h"
#include "../actor/Stage.h"
#include "../ui/Button.h"
#include "../ui/HUD.h"
#include "../../../libraries/rapidjson/document.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

Game::Game()
	: mEndFlag(1), mUpdatingActors(false)
{
}

int Game::clientID;

bool Game::Initialize(int argc, char *argv[])
{
	char name[MAX_CLIENTS][MAX_NAME_SIZE];
	char localHostName[] = "localhost";
	char *serverName;
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
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	/*データの読み込み*/
	LoadData();

	/* ネットワークイベント処理スレッドの作成 */
	thr = SDL_CreateThread(Game::NetworkEvent, "NetworkThread", &mEndFlag);

	mTicksCount = SDL_GetTicks();

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		mRacer[i] = new Racer(this, i);
	}
	mPlayer = new Player(this, clientID);

	class Stage *stage = new Stage(this);

	stage->SetStatrtPosition();


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
	UnloadData();
	mWindow->DestroyWindow();
	mNet->CloseSoc();

	SDL_Quit();
}

/*新しいUIを動的配列に追加する関数*/
void Game::PushUI(Canvas* canvas)
{
	mUIStack.emplace_back(canvas);
}

Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if(iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font *font = new Font(this);
		if(font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

void Game::LoadText(const std::string& fileName)
{
	// Clear the existing map, if already loaded
	mText.clear();
	// Try to open the file
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}
	// Read the entire file to a string stream
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// Open this file in rapidJSON
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}
	// Parse the text map
	const rapidjson::Value &actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		 itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(),
						  itr->value.GetString());
		}
	}
}

const std::string &Game::GetText(const std::string &key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	// Find this text in the map, if it exists
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}
//private

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mCommand->SendEndCommand();
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

	mCommand->SendPosCommand();
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		Vector2 pos;
		float rot;
		pos.x = mCommand->PlayerPos[i].x;
		pos.y = mCommand->PlayerPos[i].y;
		rot = mCommand->PlayerPos[i].rot;
		mRacer[i]->SetPosition(pos);
		mRacer[i]->SetRotation(rot);
	}

	if (mCommand->isCollision == true)
	{
		Vector2 pos;
		pos.x = mCommand->PlayerPos[clientID].x;
		pos.y = mCommand->PlayerPos[clientID].y;
		mPlayer->SetPosition(pos);
		mPlayer->SetRotation(mCommand->PlayerPos[clientID].rot);
		mCommand->isCollision = false;
	}

	// Add any dead actors to a temp vector
	std::vector<Actor *>
		deadActors;
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

	// UIの更新
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == Canvas::EActive)
		{
			ui->Update(deltaTime);
		}
	}
	// 描画終了したUIの削除
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == Canvas::EClosing)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
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
}

void Game::GenerateOutput()
{
	mWindow->Draw();
}

/*外部のデータを読み込む関数*/
void Game::LoadData()
{
	LoadText("assets/texts/English.gptext");
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
}

/*****************************************************************
関数名  : NetworkEvent
機能    : ネットワークのイベントループ
引数    : void		*data		: 終了判定フラグ
出力    : スレッド終了時に０を返す
*****************************************************************/
int Game::NetworkEvent(void *data)
{
	int *networkEndFlag;

	networkEndFlag = (int *)data;
	while (*networkEndFlag)
	{
		*networkEndFlag = Client_net::SendRecvManager();
	}
	return 0;
}