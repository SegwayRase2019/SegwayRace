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

#define PORT (u_short)8888 /* ポート番号 */

#define _OE_SOCKETS

#define MAX_CLIENTS 4    /* クライアント数の最大値 */
#define MAX_NAME_SIZE 10 /* ユーザー名の最大値*/

#define MAX_DATA 20 /* 送受信するデータの最大値 */

#define END_COMMAND 'E'     /* プログラム終了コマンド */
#define CIRCLE_COMMAND 'C'  /* 円表示コマンド */
#define RECT_COMMAND 'R'    /* 四角表示コマンド */
#define DIAMOND_COMMAND 'D' /* 菱形表示コマンド */

#define PLAYER_UP_COMMAND 'U' /*お試し-座標を上に上げる*/
typedef struct
{
    int x = 0;
    int y = 0;
    int rot = 0;
    int Client_id;
    char Command;
} CONTAINER;
