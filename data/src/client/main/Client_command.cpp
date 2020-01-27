#include "./Client_func.h"
#include "../../common/math/Math.h"
#include "../actor/Player.h"
#include "../component/MoveComponent.h"
#include "../ui/HUD.h"
#include "../actor/ItemBox.h"
#include "../actor/Actor.h"
#include "Music.h"

CONTAINER Posdata;
CONTAINER Client_command::PlayerPos[MAX_CLIENTS];
bool Client_command::isCollision;
bool Client_command::isPreparing;
bool Client_command::isStart;
bool Client_command::isGoal[MAX_CLIENTS];
bool Client_command::isFinish = false;
CONTAINER Client_command::PlayerPosCopy[MAX_CLIENTS];
CONTAINER Client_command::CollisionPos[MAX_CLIENTS];
Vector2 Client_command::CollisionVector[MAX_CLIENTS];
bool Client_command::isRepulsion = false;
float Client_command::Back_speed = 0.0f;
int Client_command::Collisioned_oppnent = -1;
float Client_command::Player_weight[MAX_CLIENTS];
bool Client_command::Oppnent = false;
bool Client_command::item_collision = false;
bool Client_command::item_exist = true;
//ITEM Idata;

Client_command::Client_command(Game *game)
    : mGame(game)
{
    isCollision = false;
    isPreparing = true;
    isStart = false;
    isFinish = false;
    for (int i = 0; i < MAX_CLIENTS; i++)
        isGoal[i] = false;
}

int Client_command::ExecuteCommand()
{

    int endFlag = 1;

    memset(&Posdata, 0, sizeof(CONTAINER));

    Client_net::RecvData(&Posdata, sizeof(Posdata));

    switch (Posdata.Command)
    {
    case END_COMMAND:
        endFlag = 0;
        break;

    case PLAYER_UP_COMMAND:
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
        CollisionPos[Posdata.Client_id].x = Posdata.x;
        CollisionPos[Posdata.Client_id].y = Posdata.y;
        break;

    case COLLISIONED:
        Collisioned_oppnent = Posdata.Client_id;
        CollisionVector[Game::clientID].x = Posdata.x;
        CollisionVector[Game::clientID].y = Posdata.y;
        break;

    case PLAYER_RANKING:
        PlayerPos[Posdata.Client_id].rank = Posdata.rank;
        break;

    case WAIT_SIGNAL:
        if (isPreparing == true)
            isPreparing = false;
        break;

    case START_SIGNAL:
        if (isStart == false)
        {
            isStart = true;
        }
        break;

    case ITEM_COLLISION:
        item_collision = Posdata.Item_effect;
        item_exist = Posdata.Item_exist;
        break;

    case GOAL_SIGNAL:
        if (isGoal[Posdata.Client_id] == false)
        {
            isGoal[Posdata.Client_id] = true;
            if (Posdata.Client_id == Game::clientID)
            {
                Sound::Goal_Music();
            }
        }
        PlayerPos[Posdata.Client_id].rank = Posdata.rank;
        break;
    case FINISH_COMMAND:
        if (isFinish == false)
        {
            isFinish = true;
            Sound::ResultMusic();
        }
        break;
    }

    return endFlag;
}

void Client_command::SendPosCommand(void)
{
    Actor::State state = mGame->mItem->GetState();

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
    Posdata.Item_effect = item_collision;
    switch (state)
    {
    case 0:
    {
        Posdata.Item_exist = true;
        break;
    }
    case 1:
    {
        Posdata.Item_exist = false;
        break;
    }
    }

    /*データの送信*/
    mClient_net->SendData(&Posdata, sizeof(CONTAINER));
}

// void Client_command::SendPosCommand_i(void)
// {
//     Vector2 pos = mGame->mItem->GetPosition();

//     memset(&Idata, 0, sizeof(ITEM));

//     //計算するところ
//     Idata.x = pos.x;
//     Idata.y = pos.y;
//     //Idata.rot = mGame->mPlayer->GetRotation();
//     Idata.Command = ITEM_COMMAND;
//     //Idata.num = Game::clientID;
//     switch (mGame->mItem->GetState())
//     {
//     case Actor::EInactive:
//         Idata.Exist = false;
//         break;
//     case Actor::EActive:
//         Idata.Exist = true;
//         break;
//     }

//     /*データの送信*/
//     mClient_net->SendData(&Idata, sizeof(ITEM));
// }

void Client_command::SendEndCommand(void)
{

    memset(&Posdata, 0, sizeof(CONTAINER));

    Posdata.Command = END_COMMAND;
    Posdata.Client_id = Game::clientID;

    /* データの送信 */
    mClient_net->SendData(&Posdata, sizeof(CONTAINER));
}

void Client_command::SendWaitSignal()
{
    memset(&Posdata, 0, sizeof(CONTAINER));
    Posdata.Command = WAIT_SIGNAL;
    Posdata.Client_id = Game::clientID;

    /* データの送信 */
    mClient_net->SendData(&Posdata, sizeof(CONTAINER));
}

void Client_command::SendStartSignal(void)
{
    memset(&Posdata, 0, sizeof(CONTAINER));
    Posdata.Command = START_SIGNAL;

    /* データの送信 */
    mClient_net->SendData(&Posdata, sizeof(CONTAINER));
}
