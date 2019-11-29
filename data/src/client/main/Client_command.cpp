#include "Client_func.h"

Client_command::Client_command(Game *game)
{
    mGame = game;
}

CONTAINER Posdata;

int Client_command::ExecuteCommand()
{

    int endFlag = 1;

    memset(&Posdata, 0, sizeof(CONTAINER));

    Client_net::RecvData(&Posdata, sizeof(Posdata));

    switch (Posdata.Command)
    {
    case END_COMMAND:
        endFlag = 0;
        printf("%c\n", Posdata.Command);
        break;

    case PLAYER_UP_COMMAND:
        printf("id=%d,x=%d,y=%d,rot=%d\n", Posdata.Client_id, Posdata.x, Posdata.y, Posdata.rot);
        break;
    }
    return endFlag;
}

void Client_command::SendPosCommand(void)
{
    unsigned char data[MAX_DATA];
    int dataSize;

    dataSize = 0;

    memset(&Posdata, 0, sizeof(CONTAINER));

    //計算するところ
    strage += 1;
    Posdata.y = strage;
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
