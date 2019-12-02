<<<<<<< HEAD
#include"./Client_func.h"

Client_command::Client_command(Game* game)
=======
#include "Client_func.h"

Client_command::Client_command(Game *game)
>>>>>>> 98288e8... ネットワークのマルチスレッド化
{
    mGame = game;
}

<<<<<<< HEAD
int Client_command::ExecuteCommand(char command)
{
    mClient_net = mGame -> GetClient_net();
    mWindow = mGame -> GetClient_window();

    int	endFlag = 1;
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif
    switch(command){
		case END_COMMAND:
			endFlag = 0;
			break;
	    case CIRCLE_COMMAND:
			RecvCircleData();
			break;
		case RECT_COMMAND:
			RecvRectangleData();
			break;
		case DIAMOND_COMMAND:
			RecvDiamondData();
			break;
=======
int Client_command::ExecuteCommand()
{

    int endFlag = 1;

    CONTAINER Posdata;

    memset(&Posdata, 0, sizeof(CONTAINER));

    Client_net::RecvData(&Posdata, sizeof(Posdata));

    switch (Posdata.Command)
    {
    case END_COMMAND:
        endFlag = 0;
        printf("%c\n", Posdata.Command);
        break;
    case CIRCLE_COMMAND:
        printf("circle_commnad\n");
        break;
    case RECT_COMMAND:
        break;
    case DIAMOND_COMMAND:
        break;
    case PLAYER_UP_COMMAND:
        printf("id=%d,x=%d,y=%d,rot=%d\n", Posdata.Client_id, Posdata.x, Posdata.y, Posdata.rot);
        break;
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    }
    return endFlag;
}

<<<<<<< HEAD
void Client_command::SendRectangleCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendRectangleCommand()\n");
#endif
    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,RECT_COMMAND,&dataSize);

    /* データの送信 */
    mClient_net->SendData(data,dataSize);
}

void Client_command::SendCircleCommand(int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("SendCircleCommand()\n");
    printf("Send Circle Command to %d\n",pos);
#endif

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,CIRCLE_COMMAND,&dataSize);
    /* クライアント番号のセット */
    SetIntData2DataBlock(data,pos,&dataSize);

    /* データの送信 */
    mClient_net->SendData(data,dataSize);
}

void Client_command::SendEndCommand(void)
{    char	data[MAX_DATA];
    int			dataSize;
=======
void Client_command::SendPosCommand(void)
{
    unsigned char data[MAX_DATA];
    int dataSize;

    dataSize = 0;

    // CONTAINER Posdata;

    //memset(&Posdata, 0, sizeof(CONTAINER));

    /*コマンドのセット*/
    SetCharData2DataBlock(data, PLAYER_UP_COMMAND, &dataSize);
    /*データの送信*/
    mClient_net->SendData(data, dataSize);
}

void Client_command::SendEndCommand(void)
{

    char data[MAX_DATA];
    int dataSize;
    //CONTAINER Posdata;
>>>>>>> 98288e8... ネットワークのマルチスレッド化

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif

<<<<<<< HEAD
    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);
    /* データの送信 */
    mClient_net->SendData(data,dataSize);
}

void Client_command::SetIntData2DataBlock(void *data,int intData,int *dataSize)
=======
    //memset(&Posdata, 0, sizeof(CONTAINER));

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data, END_COMMAND, &dataSize);

    /* データの送信 */
    mClient_net->SendData(data, dataSize);
}

void Client_command::SetIntData2DataBlock(void *data, int intData, int *dataSize)
>>>>>>> 98288e8... ネットワークのマルチスレッド化
{
    int tmp;

    /* 引き数チェック */
<<<<<<< HEAD
    assert(data!=NULL);
    assert(0<=(*dataSize));
=======
    assert(data != NULL);
    assert(0 <= (*dataSize));
>>>>>>> 98288e8... ネットワークのマルチスレッド化

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
<<<<<<< HEAD
    memcpy(data + (*dataSize),&tmp,sizeof(int));
=======
    memcpy(data + (*dataSize), &tmp, sizeof(int));
>>>>>>> 98288e8... ネットワークのマルチスレッド化
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

<<<<<<< HEAD
void Client_command::SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));
=======
void Client_command::SetCharData2DataBlock(void *data, char charData, int *dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));
>>>>>>> 98288e8... ネットワークのマルチスレッド化

    /* char 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}
<<<<<<< HEAD

void Client_command::RecvCircleData(void)
{
    int	x,y,r;

    /* 円コマンドに対する引き数を受信する */
    mClient_net->RecvIntData(&x);
    mClient_net->RecvIntData(&y);
    mClient_net->RecvIntData(&r);
}

void Client_command::RecvRectangleData(void)
{
    int	x,y,width,height;

    /* 四角コマンドに対する引き数を受信する */
    mClient_net->RecvIntData(&x);
    mClient_net->RecvIntData(&y);
    mClient_net->RecvIntData(&width);
    mClient_net->RecvIntData(&height);
}

void Client_command::RecvDiamondData(void)
{
    int	x,y,height;

    /* 菱形コマンドに対する引き数を受信する */
    mClient_net->RecvIntData(&x);
    mClient_net->RecvIntData(&y);
    mClient_net->RecvIntData(&height);
}
=======
>>>>>>> 98288e8... ネットワークのマルチスレッド化
