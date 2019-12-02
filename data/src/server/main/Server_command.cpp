#include "Server_common.h"
#include"Server_func.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

Server_command::Server_command(Server_net *net)
:mServer_net(net)
{
}

int Server_command::ExecuteCommand(char command, int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

    switch(command){
	    case END_COMMAND:
			dataSize = 0;
			printf("command=%c\n",command);
			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);

			/* 全ユーザーに送る */
			mServer_net->SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 0;
			break;
	    case CIRCLE_COMMAND:
			/* 円を表示するクライアント番号を受信する */
			mServer_net->RecvIntData(pos,&intData);

			dataSize = 0;
			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);
			/* 左上の x 座標のセット */
			SetIntData2DataBlock(data,GetRandomInt(500),&dataSize);
			/* 左上の y 座標のセット */
			SetIntData2DataBlock(data,GetRandomInt(300),&dataSize);
			/* 直径のセット */
			SetIntData2DataBlock(data,GetRandomInt(100),&dataSize);

			/* 指定されたクライアントに送る */
			mServer_net->SendData(intData,data,dataSize);
			break;
	    case RECT_COMMAND:
			dataSize = 0;
			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);
			/* 四角の左上の x 座標 */
			SetIntData2DataBlock(data,GetRandomInt(500),&dataSize);
			/* 四角の左上の y 座標 */
			SetIntData2DataBlock(data,GetRandomInt(300),&dataSize);
			/* 四角の横幅 */
			SetIntData2DataBlock(data,GetRandomInt(100),&dataSize);
			/* 四角の高さ */
			SetIntData2DataBlock(data,GetRandomInt(100),&dataSize);

			/* 全ユーザーに送る */
			mServer_net->SendData(ALL_CLIENTS,data,dataSize);
			break;
	    default:
			/* 未知のコマンドが送られてきた */
			fprintf(stderr,"0x%02x is not command!\n",command);
			break;
    }
    return endFlag;
}

void Server_command::SendDiamondCommand(void)
{
    unsigned char data[MAX_DATA];
    int           dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendDiamondCommand\n");
#endif
    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,DIAMOND_COMMAND,&dataSize);
    /* 菱形の左上の x 座標 */
    SetIntData2DataBlock(data,GetRandomInt(500),&dataSize);
    /* 菱形の左上の y 座標 */
    SetIntData2DataBlock(data,GetRandomInt(300),&dataSize);
    /* 菱形の高さ */
    SetIntData2DataBlock(data,GetRandomInt(100),&dataSize);

    /* クライアントに送る */
    mServer_net->SendData(ALL_CLIENTS,data,dataSize);
}

void Server_command::SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

void Server_command::SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

int Server_command::GetRandomInt(int n)
{
    return rand()%n;
}