#include "Server_calculate.h"
#include "Server_func.h"

int Restitution::Player_restitution(CONTAINER Posdata)
{
    float v1;    //衝突した方の速度
    float v2;    //衝突された方の速度
    float mv1;   //衝突後の衝突した方の速度
    float mv2;   //衝突後の衝突された方の速度
    float m1;    //衝突した方の質量
    float m2;    //衝突された方の質量
    float e = 1; //0<=e<=1の間の反発係数

    mv1 = ((m1 - e * m2) * v1 + (m2 + e * m2) * v2) / (m1 + m2);
    mv2 = ((m2 - e * m1) * v2 - (m1 + e * m1) * v1) / (m1 + m2);
}