#include "Client_func.h"

Client_command::Client_command(Game *game)
{
    mGame = game;
}

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
    }
    return endFlag;
}

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

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif

    //memset(&Posdata, 0, sizeof(CONTAINER));

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data, END_COMMAND, &dataSize);

    /* データの送信 */
    mClient_net->SendData(data, dataSize);
}

void Client_command::SetIntData2DataBlock(void *data, int intData, int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize), &tmp, sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

void Client_command::SetCharData2DataBlock(void *data, char charData, int *dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 <= (*dataSize));

    /* char 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}
