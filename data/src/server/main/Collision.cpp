#include "Collision.h"

bool Collision::collision;
CONTAINER Collision::PlayerPos[MAX_CLIENTS];
float Collision::distance;
int Collision::collision_oppnent;
int Collision::Player_Collision_Strage[MAX_CLIENTS];
float Collision::Player_Speed[MAX_CLIENTS];
int Collision::Collisioned_opponent[MAX_CLIENTS];
Vector2 Collision::Collision_Vector[MAX_CLIENTS];
Vector2 Collision::Item_coordinate;
bool Collision::Item_effect = false;

int Collision::Collision_Judgement(CONTAINER Posdata)
{
    Player_Collision(Posdata); //プレイヤーの衝突判定をする
    //Collision_item(Posdata);//アイテムの衝突判定をする
    Stage_Collision(Posdata); //ステージの当たり判定を計算する

    //衝突判定があったどうかを確認する
    if (Player_Collision_Strage[Posdata.Client_id] == 1)
    {

        int Client_id = Server_command::Posdata.Client_id;
        Posdata.Command = COLLISIONED;
        Posdata.Client_id = Collisioned_opponent[Client_id];
        Posdata.x = Collision_Vector[Client_id].x;
        Posdata.y = Collision_Vector[Client_id].y;
        Server_net::SendData(Client_id, &Posdata, sizeof(CONTAINER));
        Posdata.Client_id = Client_id;
        Server_command::Posdata.Command = PLAYER_COLLISION;
        Server_command::Posdata.speed = Calculate::SpeedStorage[Server_command::Posdata.Client_id];
        Player_Collision_Strage[Posdata.Client_id] = 0;
    }
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
        PlayerPos[Posdata.Client_id].speed = Posdata.speed;
        PlayerPos[Posdata.Client_id].weight = Posdata.weight;
    }

    if (collision == true)
    {
        if (Server_command::Goal_Status[collision_oppnent] != true)
        {
            printf("ID%dがID%dに衝突\n", Posdata.Client_id, collision_oppnent);
            Server_command::Posdata.Command = PLAYER_COLLISION;
            Calculate::v2 = PlayerPos[collision_oppnent].speed;
            Collisioned_opponent[collision_oppnent] = Posdata.Client_id;
            Collision_Vector[collision_oppnent].x = PlayerPos[Posdata.Client_id].x - Posdata.x;
            Collision_Vector[collision_oppnent].y = PlayerPos[Posdata.Client_id].y - Posdata.y;
            Calculate::m2 = PlayerPos[collision_oppnent].weight; //衝突された方の質量
            Calculate::Player_restitution(Posdata);
        }
    }
    collision = false;
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
        {
            Server_command::Posdata.Command = PLAYER_COLLISION;
            collision = true;
        }
    }
    else if (Stage_buttom.y <= Posdata.y) //buttomのほうが近いとき
    {
        if (distance_buttom >= D || distance_buttom <= d)
        {
            Server_command::Posdata.Command = PLAYER_COLLISION;
            collision = true;
        }
    }
    else if (Stage_upper.x < Posdata.x) //ステージの右側の当たり判定計算
    {
        if (Stage_upper.x + D < Posdata.x || Stage_upper.x + d > Posdata.x)
        {
            Server_command::Posdata.Command = PLAYER_COLLISION;
            collision = true;
        }
    }
    else if (Stage_upper.x > Posdata.x) //ステージの左側の当たり判定計算
    {
        if (Stage_upper.x - D > Posdata.x || Stage_upper.x - d < Posdata.x)
        {
            Server_command::Posdata.Command = PLAYER_COLLISION;
            collision = true;
        }
    }
    if (collision == true)
    {
        Calculate::v2 = 0;
        collision_oppnent = -1;
        Calculate::m2 = PlayerPos[Posdata.Client_id].weight;
        Calculate::Player_restitution(Posdata);
    }
}

int Collision::Collision_item(CONTAINER Posdata)
{
    //アイテムの座標を入れる
    Item_coordinate.x = 0;
    Item_coordinate.y = 0;
    Item_effect = false;

    float radious = 70; //半径

    float a = std::abs(Posdata.x - Item_coordinate.x);
    float b = std::abs(Posdata.y - Item_coordinate.y);

    float Item_distance = std::sqrt((a * a) + (b * b));

    if (Item_distance < radious)
    {
        Item_effect = true;
    }
}