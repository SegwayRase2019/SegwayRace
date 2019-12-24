#include "Server_calculate.h"
#include "Collision.h"
#include "Server_func.h"
#include <math.h>
#include <algorithm>
#include <vector>

float Calculate::Player_angle[MAX_CLIENTS];
float Calculate::Player_angle_copy[MAX_CLIENTS];
float Calculate::Player_laps[MAX_CLIENTS] = {0, 0, 0, 0};
bool Calculate::Warnig_mode = false;
float Calculate::Before_degree[MAX_CLIENTS];
float Calculate::CollisionStrage[MAX_CLIENTS] = {0, 0, 0, 0};
float Calculate::v2 = 0; //衝突された方の速度
float Calculate::SpeedStorage[MAX_CLIENTS];
float Calculate::m2;

int Calculate::Player_restitution(CONTAINER Posdata)
{
    float v1 = Server_command::Posdata.speed;                  //衝突した方の速度
    float mv1 = 0;                                             //衝突後の衝突した方の速度
    float mv2 = 0;                                             //衝突後の衝突された方の速度
    float m1 = Collision::PlayerPos[Posdata.Client_id].weight; //衝突した方の質量
    float e = 0.5f;                                            //0<=e<=1の間の反発係数

    mv1 = ((m1 - e * m2) * v1 + (m2 + e * m2) * v2) / (m1 + m2);
    mv2 = ((m2 - e * m1) * v2 - (m1 + e * m1) * v1) / (m1 + m2);
    Server_command::Posdata.speed = mv1;

    SpeedStorage[Collision::collision_oppnent] = mv2;

    Collision::Player_Collision_Strage[Collision::collision_oppnent] = 1;
    printf("m1 = %lf,m2 = %lf\n", m1, m2);
}

int Calculate::Stage_rank(CONTAINER Posdata) //反時計回りを想定
{

    Calculate_angle();
    std::sort(Player_angle, Player_angle + SIZE_OF_ANGLE(Player_angle), std::greater<float>()); //降順にソート
    for (int j = 0; j < Server_net::gClientNum; j++)
    {

        if (Player_angle_copy[Posdata.Client_id] == Player_angle[j])
        {
            Server_command::Posdata.rank = j + 1;
            break;
        }
    }
}

//角度を計算する
int Calculate::Calculate_angle()
{
    Vector2 Stage_middle; //ステージの中心座標
    Stage_middle.x = 0;
    Stage_middle.y = 0;
    bool Back_run = false;

    for (int i = 0; i < Server_net::gClientNum; i++)
    {
        float radian = std::atan2(Stage_middle.y - Collision::PlayerPos[i].y, Collision::PlayerPos[i].x - Stage_middle.x);
        float degree = radian * 180 / 3.14;

        //１８０度のところで逆走した場合の角度の調整

        if (degree > 175 && Before_degree[i] < -175)
        {
            Warnig_mode = true;

            Back_run = true;
            Player_laps[i]--;
        }

        //180度のときの処理
        if (degree < -175 && Before_degree[i] > 175)
        {
            Player_laps[i]++;
        }

        Player_angle[i] = degree + (360 * Player_laps[i]);
        Player_angle_copy[i] = degree + (360 * Player_laps[i]);
        Before_degree[i] = degree;
    }
}