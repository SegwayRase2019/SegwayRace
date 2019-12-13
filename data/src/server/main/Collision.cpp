#include "Collision.h"

bool Collision::collision;
CONTAINER Collision::PlayerPos[MAX_CLIENTS];
float Collision::distance;
int Collision::collision_oppnent;

int Collision::Collision_Judgement(CONTAINER Posdata)
{
    Player_Collision(Posdata); //プレイヤーの衝突判定をする
    Stage_Collision(Posdata);  //ステージの当たり判定を計算する
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

int Collision::Stage_Collision(CONTAINER Posdata)
{
    //条件:buttomとupperのx座標は同一であること
    Vector2 Stage_buttom;
    Stage_buttom.x = 0;
    Stage_buttom.y = 1800;
    Vector2 Stage_upper;
    Stage_upper.x = 0;
    Stage_upper.y = -1800;

    float D = 1750; //ステージの外側の幅
    float d = 950;  //ステージの内側の幅

    //buttomとupperとプレイヤーの距離計算
    float distance_upper, distance_buttom;

    float a = std::abs(Posdata.x - Stage_upper.x);
    float b = std::abs(Posdata.y - Stage_upper.y);
    distance_upper = std::sqrt((a * a) + (b * b));

    float c = std::abs(Posdata.x - Stage_buttom.x);
    float e = std::abs(Posdata.y - Stage_buttom.y);
    distance_buttom = std::sqrt((c * c) + (e * e));

    //２つの距離のどちらが近いか計算する
    if (Stage_upper.y >= Posdata.y) //upperのほうが近いとき
    {
        if (distance_upper >= D || distance_upper <= d)
            Server_command::Posdata.Command = PLAYER_COLLISION;
    }
    else if (Stage_buttom.y <= Posdata.y) //buttomのほうが近いとき
    {
        if (distance_buttom >= D || distance_buttom <= d)
            Server_command::Posdata.Command = PLAYER_COLLISION;
    }
    else if (Stage_upper.x < Posdata.x) //ステージの右側の当たり判定計算
    {
        if (Stage_upper.x + D < Posdata.x || Stage_upper.x + d > Posdata.x)
            Server_command::Posdata.Command = PLAYER_COLLISION;
    }
    else if (Stage_upper.x > Posdata.x) //ステージの左側の当たり判定計算
    {
        if (Stage_upper.x - D > Posdata.x || Stage_upper.x - d < Posdata.x)
            Server_command::Posdata.Command = PLAYER_COLLISION;
    }
}