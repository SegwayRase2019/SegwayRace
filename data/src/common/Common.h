/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <SDL2/SDL.h>
#include "./math/Math.h"

#define PORT (u_short)8888 /* ポート番号 */

#define _OE_SOCKETS

#define MAX_CLIENTS 4    /* クライアント数の最大値 */
#define MAX_NAME_SIZE 10 /* ユーザー名の最大値*/

#define MAX_DATA 20 /* 送受信するデータの最大値 */

#define END_COMMAND 'E' /* プログラム終了コマンド */

#define PLAYER_UP_COMMAND 'U' /*お試し-座標を上に上げる*/
#define PLAYER_COLLISION 'P'  /*プレイヤーが衝突した判定*/
#define COLLISIONED 'C'
#define PLAYER_RANKING 'R' /*プレイヤのランキング*/
#define START_SIGNAL 'S' /*レースがスタートした事を通知*/
#define GOAL_SIGNAL 'G' /*プレイヤがゴールした事を通知*/


typedef struct
{
    float x = 0;
    float y = 0;
    float rot = 0;
    int Client_id;
    char Command;
    int rank;
    float speed;
    float weight = 50;
} CONTAINER;

typedef struct
{
    float wlt;
    float wrt;
    float wlb;
    float wrb;
    float bal_x;
    float bal_y;
    float weight;
} Prs;

