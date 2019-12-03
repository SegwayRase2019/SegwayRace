#include "Server_common.h"
#include "Server_func.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Server_command::Server_command(Server_net *net)
	: mServer_net(net)
{
}

static CONTAINER Posdata;

int Server_command::ExecuteCommand(int pos)
{
	unsigned char data[MAX_DATA];
	int dataSize, intData;
	int endFlag = 1;

	memset(&Posdata, 0, sizeof(CONTAINER));

	Server_net::RecvData(pos, &Posdata, sizeof(Posdata));

	switch (Posdata.Command)
	{
	case END_COMMAND:
	{
		dataSize = 0;
		printf("command=%c\n", Posdata.Command);

		/* 全ユーザーに送る */
		Server_net::SendData(ALL_CLIENTS, &Posdata, sizeof(CONTAINER));

		endFlag = 0;
		break;
	}
	case PLAYER_UP_COMMAND:
	{

		/*クライアント側に構造体を送る*/
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