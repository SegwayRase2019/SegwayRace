#include "./Client_func.h"

Client_command::Client_command(Game *game)
{
    mGame = game;
}

int Client_command::ExecuteCommand(char command)
{
    mClient_net = mGame->GetClient_net();
    mWindow = mGame->GetClient_window();

    int endFlag = 1;
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n", command);
#endif
    switch (command)
    {
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
    }
    return endFlag;
}

void Client_command::SendRectangleCommand(void)
{
    unsigned char data[MAX_DATA];
    int dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendRectangleCommand()\n");
#endif
    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data, RECT_COMMAND, &dataSize);

    /* データの送信 */
    mClient_net->SendData(data, dataSize);
}

void Client_command::SendCircleCommand(int pos)
{
    unsigned char data[MAX_DATA];
    int dataSize;

    /* 引き数チェック */
    assert(0 <= pos && pos < MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("SendCircleCommand()\n");
    printf("Send Circle Command to %d\n", pos);
#endif

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data, CIRCLE_COMMAND, &dataSize);
    /* クライアント番号のセット */
    SetIntData2DataBlock(data, pos, &dataSize);

    /* データの送信 */
    mClient_net->SendData(data, dataSize);
}

void Client_command::SendEndCommand(void)
{
    char data[MAX_DATA];
    int dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif

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

void Client_command::RecvCircleData(void)
{
    int x, y, r;

    /* 円コマンドに対する引き数を受信する */
    mClient_net->RecvIntData(&x);
    mClient_net->RecvIntData(&y);
    mClient_net->RecvIntData(&r);
}

void Client_command::RecvRectangleData(void)
{
    int x, y, width, height;

    /* 四角コマンドに対する引き数を受信する */
    mClient_net->RecvIntData(&x);
    mClient_net->RecvIntData(&y);
    mClient_net->RecvIntData(&width);
    mClient_net->RecvIntData(&height);
}

void Client_command::RecvDiamondData(void)
{
    int x, y, height;

    /* 菱形コマンドに対する引き数を受信する */
    mClient_net->RecvIntData(&x);
    mClient_net->RecvIntData(&y);
    mClient_net->RecvIntData(&height);
}
