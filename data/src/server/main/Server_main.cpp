#include "./Server_common.h"
#include "./Server_func.h"
#include <iostream>

int main(int argc, char *argv[])
{
	Server_net *net;

	net = new Server_net();
	SDL_Thread *thr;

	int num;
	int endFlag = 1;

	/* 引き数チェック */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: number of clients\n");
		exit(-1);
	}
	if ((num = atoi(argv[1])) < 0 || num > MAX_CLIENTS)
	{
		fprintf(stderr, "clients limit = %d \n", MAX_CLIENTS);
		exit(-1);
	}

	/* SDLの初期化 */
	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* クライアントとの接続 */
	if (net->SetUpServer(num) == -1)
	{
		fprintf(stderr, "Cannot setup server\n");
		exit(-1);
	}

	/* メインイベントループ */
	while (endFlag)
	{
		endFlag = Server_net::running;
	};

	/* 終了処理 */
	net->Ending();

	return 0;
}
