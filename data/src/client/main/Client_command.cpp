#include "./Client_func.h"
#include "../../common/math/Math.h"
#include "../actor/Player.h"

Client_command::Client_command(Game *game)
{
    mGame = game;
    isCollision = false;
}

CONTAINER Posdata;
CONTAINER Client_command::PlayerPos[MAX_CLIENTS];
bool Client_command::isCollision;

int Client_command::ExecuteCommand()
{

    int endFlag = 1;

    memset(&Posdata, 0, sizeof(CONTAINER));

    Client_net::RecvData(&Posdata, sizeof(Posdata));

    if (Posdata.Client_id == Game::clientID)
    {
        printf("Rank = %d\n", Posdata.rank);
    }

    switch (Posdata.Command)
    {
    case END_COMMAND:
        endFlag = 0;
        printf("%c\n", Posdata.Command);
        break;

    case PLAYER_UP_COMMAND:
        printf("id=%d,x=%f,y=%f,rot=%f\n", Posdata.Client_id, Posdata.x, Posdata.y, Posdata.rot);
        PlayerPos[Posdata.Client_id].x = Posdata.x;
        PlayerPos[Posdata.Client_id].y = Posdata.y;
        PlayerPos[Posdata.Client_id].rot = Posdata.rot;
        PlayerPos[Posdata.Client_id].Client_id = Posdata.Client_id;
        break;

    case PLAYER_COLLISION:
        isCollision = true;
        break;
    }
    return endFlag;
}

void Client_command::SendPosCommand(void)
{
    Vector2 pos = mGame->mPlayer->GetPosition();

    memset(&Posdata, 0, sizeof(CONTAINER));

    //計算するところ
    Posdata.x = pos.x;
    Posdata.y = pos.y;
    Posdata.rot = mGame->mPlayer->GetRotation();
    Posdata.Command = PLAYER_UP_COMMAND;
    Posdata.Client_id = Game::clientID;

    /*データの送信*/
    mClient_net->SendData(&Posdata, sizeof(CONTAINER));
}

void Client_command::SendEndCommand(void)
{

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif

    memset(&Posdata, 0, sizeof(CONTAINER));

    Posdata.Command = END_COMMAND;
    Posdata.Client_id = Game::clientID;

    /* データの送信 */
    mClient_net->SendData(&Posdata, sizeof(CONTAINER));
}