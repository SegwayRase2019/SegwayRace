#include "./Game.h"
#include <SDL2/SDL.h>

cwiid_mesg_callback_t cwiid_callback;

int main(int argc, char *argv[])
{
    Game *game = new Game();

    bool Wii_Fit_connect = false;
    int Wiiflag = 1;
    SDL_Thread *Wii_thr;

    /* 初期化 */
    if (!(game->Initialize(argc, argv)))
    {
        SDL_Log("fatal:Game.Initialize\n");
        return 0;
    }

    /* メインループ */
    while (game->GetEndFlag())
    {
        game->RunLoop();
    };

    /* 終了処理 */
    game->Shutdown();

    return 0;
}
