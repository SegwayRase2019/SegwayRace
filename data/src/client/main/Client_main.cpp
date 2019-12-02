#include "./Game.h"
#include<SDL2/SDL.h>

int main(int argc,char *argv[])
{
    Game* game = new Game();

    /* 初期化 */
    if(!(game -> Initialize(argc, argv))){
        SDL_Log("fatal:Game.Initialize\n");
        return 0;
    }
	
	/* メインループ */
    while(game -> GetEndFlag()){
		game -> RunLoop();
    };

    /* 終了処理 */
    game -> Shutdown();

    return 0;
}
