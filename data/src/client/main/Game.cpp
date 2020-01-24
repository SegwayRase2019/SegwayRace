#include "Game.h"
#include "./Client_window.h"
#include "../actor/Racer.h"
#include "../actor/Player.h"
#include "../actor/Stage.h"
#include "../ui/Button.h"
#include "../ui/HUD.h"
#include "../ui/Resultwindow.h"
#include "../actor/ItemBox.h"
#include "../../../libraries/rapidjson/document.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <SDL2/SDL_mixer.h>
#include <fstream>
#include <sstream>
#include "Music.h"

Game::Game()
	: mEndFlag(1), mWiiFlag(1), mUpdatingActors(false), mIntervalTime(0.2f), mCountTimer(0), mGameState(ERunning)
{
}

int Game::clientID;
CONTAINER Game::Player_difference[MAX_CLIENTS];
CONTAINER Game::Collision_difference[MAX_CLIENTS];
int Game::volume = 50;

Prs Game::prs;

char Game::command;
bool Start_BGM = false;

cwiid_wiimote_t *wiimote = NULL; //WiiBalanceBoardの情報

bool Game::Initialize(int argc, char *argv[])
{
	char name[MAX_CLIENTS][MAX_NAME_SIZE];
	char localHostName[] = "localhost";
	char *serverName;
	class Client_command *command;

	mNet = new Client_net(this);
	mWindow = new Client_window(this);
	mCommand = new Client_command(this);

	bdaddr_t bdaddr;				//一時保管用BluetoothのIDボックス
	struct balance_cal balance_cal; //各バランスボードの調整用の値の取得

	/* 引き数チェック */
	if (argc == 1)
	{
		serverName = localHostName;
	}
	else if (argc == 2)
	{
		serverName = argv[1];
	}
	else if (argc == 3)
	{
		serverName = argv[1];
		str2ba(argv[3], &bdaddr); //要調整必要、引数の取ったアドレスで渡す
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

	mHUD = new HUD(this);

	stage->SetStatrtPosition();

	class Sound *sound = new Sound(this);
	sound->Sound_Initialize();

	mItem = new ItemBox(this);
	Vector2 itemPos;
	itemPos.x = 1500;
	itemPos.y = 0;
	mItem->SetPosition(itemPos);
	mPlayer->SetPlayerState(Player::PlayerState::ERunning); // 後で消す

	//ここからwiifitの初期化

	if ((wiimote = cwiid_open(&bdaddr, 0)) == NULL) //コネクトに必要な関数
	{
		fputs("Unable to connect\n", stderr);
		wiifit_connect = false;
		Client_command::Player_weight[clientID] = 50;
		return true;
	}
	fputs("Wiifit_connected\n", stdout);

	sleep(1); //一秒止まる(ここは消しても良い)

	if (cwiid_set_led(wiimote, 1)) //正直どっちでもいい
		fputs("Unable to set LED state\n", stderr);

	if (cwiid_get_balance_cal(wiimote, &balance_cal)) //これはバグっていたら大変
		fputs("unable to retrieve balance calibration\n", stderr);

	if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) //これは必須
		fputs("cannot set callback. buttons won't work.\n", stderr);

	if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) //これも接続に必須
		fputs("cannot enable callback. buttons won't work.\n", stderr);

	if (cwiid_set_rpt_mode(wiimote,
						   CWIID_RPT_ACC | CWIID_RPT_STATUS | CWIID_RPT_EXT)) //これも必須
		fputs("cannot set report mode. buttons won't work.\n", stderr);

	if (wiifit_connect == true)
	{
		wii_fit_thr = SDL_CreateThread(Game::Wiifit_Thread, "Wiifit_Thread", &mWiiFlag);
	}

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
void Game::PushUI(Canvas *canvas)
{
	mUIStack.emplace_back(canvas);
}

Font *Game::GetFont(const std::string &fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font *font = new Font(this);
		if (font->Load(fileName))
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

void Game::LoadText(const std::string &fileName)
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
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				volume++;
				if (volume > 128)
				{
					volume = 128;
				}
				break;
			case SDLK_DOWN:
				volume--;
				if (volume < 0)
				{
					volume = 0;
				}
				break;
			default:
				break;
			}
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

	//Update music volume
	Sound::UpdateMusicVolume();

	// Update all actors
	if (mGameState == ERunning)
	{
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

		if (Client_command::isRepulsion == true && mPlayer->GetPlayerState() == Player::PlayerState::ERunning)
		{
			Vector2 pos;

			if (Client_command::Collisioned_oppnent == -1)
			{
				mCommand->PlayerPos[clientID].x -= Client_command::Back_speed * Player_difference[clientID].x * deltaTime * 0.5f;
				mCommand->PlayerPos[clientID].y -= Client_command::Back_speed * Player_difference[clientID].y * deltaTime * 0.5f;
			}
			else
			{
				mCommand->PlayerPos[clientID].x -= Client_command::Back_speed * Collision_difference[clientID].x * deltaTime * 0.5f;
				mCommand->PlayerPos[clientID].y -= Client_command::Back_speed * Collision_difference[clientID].y * deltaTime * 0.5f;
			}
			pos.x = mCommand->PlayerPos[clientID].x;
			pos.y = mCommand->PlayerPos[clientID].y;
			mPlayer->SetPosition(pos);

			mCountTimer += deltaTime;
			if (mCountTimer > mIntervalTime)
			{
				Client_command::isRepulsion = false;
				Client_command::Collisioned_oppnent = -1;
				mCountTimer = 0;
				Sound::Collision_Sound();

				if (Client_command::isCollision == true)
				{
					mCommand->PlayerPos[clientID].x = Client_command::PlayerPosCopy[clientID].x;
					mCommand->PlayerPos[clientID].y = Client_command::PlayerPosCopy[clientID].y;
					pos.x = mCommand->PlayerPos[clientID].x;
					pos.y = mCommand->PlayerPos[clientID].y;
					mPlayer->SetPosition(pos);
					mRacer[clientID]->SetPosition(pos);
				}
			}
		}

		if (mCommand->isCollision == true)
		{
			Vector2 pos;
			if (Client_command::Collisioned_oppnent == -1)
			{
				Player_difference[clientID].x = mCommand->CollisionPos[clientID].x - mCommand->PlayerPos[clientID].x;
				Player_difference[clientID].y = mCommand->CollisionPos[clientID].y - mCommand->PlayerPos[clientID].y;
			}
			else
			{
				Collision_difference[clientID].x = mCommand->CollisionVector[clientID].x;
				Collision_difference[clientID].y = mCommand->CollisionVector[clientID].y;
			}

			pos.x = mCommand->PlayerPos[clientID].x;
			pos.y = mCommand->PlayerPos[clientID].y;
			mPlayer->SetPosition(pos);
			mPlayer->SetRotation(mCommand->PlayerPos[clientID].rot);

			mCommand->isCollision = false;
			Client_command::isRepulsion = true;
			if (mCommand->isStart == true)
				mPlayer->SetPlayerState(Player::PlayerState::ERunning);
			if (mCommand->isGoal[clientID] == true)
			{
				mPlayer->SetPlayerState(Player::PlayerState::EGoal);
				mCommand->isStart = false;
			}

			pos.x = mCommand->PlayerPos[clientID].x;
			pos.y = mCommand->PlayerPos[clientID].y;
			mPlayer->SetPosition(pos);
			mPlayer->SetRotation(mCommand->PlayerPos[clientID].rot);

			mCommand->isCollision = false;
			Client_command::isRepulsion = true;
		}
		if (mCommand->isStart == true)
		{
			mPlayer->SetPlayerState(Player::PlayerState::ERunning);

			if (Start_BGM == false)
			{
				class Sound *sound = new Sound(this);
				sound->BackGroundMusic();
				Start_BGM = true;
			}
		}
		if (mCommand->isGoal[clientID] == true)
		{
			mPlayer->SetPlayerState(Player::PlayerState::EGoal);
			mCommand->isStart = false;
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

	if (mCommand->isFinish == true && mGameState == ERunning)
	{
		class Resultwindow *result = new Resultwindow(this);
		mGameState = EFinished;
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

void Game::cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
						  union cwiid_mesg mesg[], struct timespec *ts) //ここは必須、thread的要素
{
	for (int i = 0; i < mesg_count; i++)
	{
		if (mesg[i].type == CWIID_MESG_BALANCE)
		{
		}
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

int Game::Wiifit_Thread(void *data)
{
	int *wiifitEndFlag;
	wiifitEndFlag = (int *)data;
	while (wiifitEndFlag)
	{
		Wii_action::centroid(wiimote, &prs);			 //重心の取得
		*wiifitEndFlag = Wii_action::move_command(&prs); //prsの情報からコマンドの判別
	}
	return 0;
}