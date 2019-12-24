#include "./Client_func.h"
#include "../../common/math/Math.h"
#include "../actor/Player.h"
#include "../component/MoveComponent.h"
#include "../ui/HUD.h"

Client_command::Client_command(Game *game)
{
    mGame = game;
    isCollision = false;
}

CONTAINER Posdata;
CONTAINER Client_command::PlayerPos[MAX_CLIENTS];
CONTAINER Client_command::PlayerPosCopy[MAX_CLIENTS];
CONTAINER Client_command::CollisionPos[MAX_CLIENTS];
Vector2 Client_command::CollisionVector[MAX_CLIENTS];
bool Client_command::isCollision;
bool Client_command::isRepulsion = false;
float Client_command::Back_speed = 0.0f;
int Client_command::Collisioned_oppnent = -1;
float Client_command::Player_weight[MAX_CLIENTS];
bool Client_command::Oppnent = false;

int Client_command::ExecuteCommand()
{

    int endFlag = 1;

    memset(&Posdata, 0, sizeof(CONTAINER));

    Client_net::RecvData(&Posdata, sizeof(Posdata));

    if (Posdata.Client_id == Game::clientID)
    {
        // printf("Rank = %d\n", Posdata.rank);
    }

    switch (Posdata.Command)
    {
    case END_COMMAND:
        endFlag = 0;
        //printf("%c\n", Posdata.Command);
        break;

    case PLAYER_UP_COMMAND:
        //printf("id=%d,x=%f,y=%f,rot=%f\n", Posdata.Client_id, Posdata.x, Posdata.y, Posdata.rot);
        PlayerPos[Posdata.Client_id].x = Posdata.x;
        PlayerPos[Posdata.Client_id].y = Posdata.y;
        PlayerPos[Posdata.Client_id].rot = Posdata.rot;
        PlayerPos[Posdata.Client_id].Client_id = Posdata.Client_id;
        PlayerPos[Posdata.Client_id].rank = Posdata.rank;
        PlayerPosCopy[Posdata.Client_id].x = Posdata.x;
        PlayerPosCopy[Posdata.Client_id].y = Posdata.y;

        break;

    case PLAYER_COLLISION:
        isCollision = true;
        Back_speed = std::abs(Posdata.speed);
        printf("backspeed=%lf\n", Back_speed);

        CollisionPos[Posdata.Client_id].x = Posdata.x;
        CollisionPos[Posdata.Client_id].y = Posdata.y;

        break;

    case COLLISIONED:
        Collisioned_oppnent = Posdata.Client_id;
        CollisionVector[Game::clientID].x = Posdata.x;
        CollisionVector[Game::clientID].y = Posdata.y;
        printf("x=%lf,y=%lf\n", CollisionVector[Game::clientID].x, CollisionVector[Game::clientID].y);
        break;

    case PLAYER_RANKING:
        PlayerPos[Posdata.Client_id].rank = Posdata.rank;
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
    Posdata.speed = MoveComponent::mForwardSpeed;
    Posdata.weight = Player_weight[Game::clientID];

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
