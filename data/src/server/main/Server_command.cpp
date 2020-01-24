#include "Server_common.h"
#include "Server_func.h"
#include "Collision.h"
#include "Server_calculate.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Server_command::Server_command(Server_net *net)
	: mServer_net(net)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		plWait[i] = false;
	}
}

CONTAINER Server_command::Posdata;
bool Server_command::Goal_Status[MAX_CLIENTS];
int Server_command::Result_Rank[MAX_CLIENTS];
int Server_command::final_rank = 0;
bool Server_command::plWait[MAX_CLIENTS];
//ITEM Server_command::Idata;

bool Server_command::debug[MAX_CLIENTS];

int Server_command::ExecuteCommand(int pos)
{
	unsigned char data[MAX_DATA];
	int dataSize, intData;
	int endFlag = 1;

	memset(&Posdata, 0, sizeof(CONTAINER));

	Server_net::RecvData(pos, &Posdata, sizeof(Posdata));

	if (Posdata.Command != END_COMMAND && Posdata.Command != WAIT_SIGNAL)
	{
		Calculate::Stage_rank(Posdata);
		Collision::Collision_Judgement(Posdata); //当たり判定
	}

	switch (Posdata.Command)
	{
	case END_COMMAND:
	{
		dataSize = 0;

		/* 全ユーザーに送る */
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));

		endFlag = 0;
		break;
	}
	case PLAYER_UP_COMMAND:
	{

		/*クライアント側に構造体を送る*/
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));
		Posdata.Command = PLAYER_RANKING;
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));

		break;
	}
	case PLAYER_COLLISION:
	{
		Server_net::SendData(Posdata.Client_id, &Posdata, sizeof(CONTAINER));
		Posdata.Command = PLAYER_RANKING;
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));
		break;
	}
	case ITEM_COLLISION:
	{
		Server_net::SendData(Posdata.Client_id, &Posdata, sizeof(CONTAINER));
		//Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));
		break;
	}
	case WAIT_SIGNAL:
	{
		plWait[Posdata.Client_id] = true;
		for (int i = 0; i < Server_net::gClientNum; i++)
		{
			if (!plWait[i])
				break;
			if (i == (Server_net::gClientNum - 1))
				Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));
		}
		break;
	}
	case START_SIGNAL:
	{
		dataSize = 0;

		/* 全ユーザーに送る */
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));

		break;
	}
	case GOAL_SIGNAL:
	{
		dataSize = 0;

		/* 全ユーザーに送る */
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));
		break;
	}
	case FINISH_COMMAND:
	{
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));
		break;
	}
	default:
		/* 未知のコマンドが送られてきた */
		//fprintf(stderr, "0x%02x is not command!\n", Posdata.Command);
		break;
	}
	return endFlag;
}