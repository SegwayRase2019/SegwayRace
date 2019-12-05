/*
 * Copyright (C) 2011 by Daniel Friesel <derf@finalrewind.org>
 * License: WTFPL <http://sam.zoy.org/wtfpl>
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 */

/* Wii_main.c */

#include "Wii_common.h"
#include <unistd.h>
#include <time.h>
#include <bluetooth/bluetooth.h>

cwiid_mesg_callback_t cwiid_callback;

int Wii_main(char *argv[]) //もとはmain,BluetoothのIDを受け取る
{
	cwiid_wiimote_t *wiimote = NULL; //WiiBalanceBoardの情報
	char command;	//転送するコマンド
	bdaddr_t bdaddr; //一時保管用BluetoothのIDボックス
	struct balance_cal balance_cal;  //各バランスボードの調整用の値の取得

	Wii_action::Prs prs;    //重心と四つの場所の重さの情報が入った構造体
	class Wii_action *act = new Wii_action();	//宣言

	str2ba(argv[1],&bdaddr); //要調整必要、引数の取ったアドレスで渡す

	if ((wiimote = cwiid_open(&bdaddr, 0)) == NULL)	//コネクトに必要な関数
	{
		fputs("Unable to connect\n", stderr);
		return EXIT_FAILURE;
	}
	fputs("connected\n", stdout);

	sleep(1); //一秒止まる(ここは消しても良い)

	if (cwiid_set_led(wiimote, 1)) //正直どっちでもいい
		fputs("Unable to set LED state\n", stderr);

	if (cwiid_get_balance_cal(wiimote, &balance_cal))	//これはバグっていたら大変
		fputs("unable to retrieve balance calibration\n", stderr);

	if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) //これは必須
		fputs("cannot set callback. buttons won't work.\n", stderr);

	if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC))	//これも接続に必須
		fputs("cannot enable callback. buttons won't work.\n", stderr);

	if (cwiid_set_rpt_mode(wiimote,
						   CWIID_RPT_ACC | CWIID_RPT_STATUS | CWIID_RPT_EXT))	//これも必須
		fputs("cannot set report mode. buttons won't work.\n", stderr);

	while (1) //動作のループ
	{
		act->centroid(wiimote,&prs); //重心の取得
		command = act->move_command(prs);	//prsの情報からコマンドの判別
		printf("command is %c\n", command);	//ここは確認用なのでけしてよい
		/**********************
		ここにコマンド転送と止まれとサーバーから受け取ったとき
		止まるようにコマンドを入れる
		(例)
		char st_command;
		send_data(client[],command);
		if(receive_data(server,st_command) == STOP_ACT){
			break;
		}
		*************************/
	}
	cwiid_close(wiimote);	//全て閉じる

	return EXIT_SUCCESS;
}

void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
								union cwiid_mesg mesg[], struct timespec *ts)	//ここは必須、thread的要素
{
	for (int i = 0; i < mesg_count; i++)
	{
		if (mesg[i].type == CWIID_MESG_BALANCE)
		{
		}
	}
}