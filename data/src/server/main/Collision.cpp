#include "Collision.h"

bool Collision::collision;
CONTAINER Collision::PlayerPos[MAX_CLIENTS];
float Collision::distance;
int Collision::collision_oppnent;

int Collision::Collision_Judgement(CONTAINER Posdata)
{
    Player_Collision(Posdata); //プレイヤーの衝突判定をする
}

int Collision::Player_Collision(CONTAINER Posdata)
{
    collision = false;

    float radius = 70; //プレイヤーの半径(当たり判定を変えたいときはここの値を変更する)

    for (int i = 0; i < Server_net::gClientNum; i++)
    {
        if (i != Posdata.Client_id)
        {
            float a = std::abs(Posdata.x + 50 - (PlayerPos[i].x + 50));
            float b = std::abs(Posdata.y + 50 - (PlayerPos[i].y + 50));

            distance = std::sqrt((a * a) + (b * b));

            if (distance < radius)
            {
                collision = true;
                collision_oppnent = i;
            }
        }
    }

    if (collision == false)
    {
        //１フレーム前のコピーを取る
        PlayerPos[Posdata.Client_id].x = Posdata.x;
        PlayerPos[Posdata.Client_id].y = Posdata.y;
    }

    if (collision == true)
    {
        printf("ID%dがID%dに衝突\n", Posdata.Client_id, collision_oppnent);
        Server_command::Posdata.Command = PLAYER_COLLISION;
    }
}