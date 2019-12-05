/* Wii_command.c */

#include "Wii_common.h"

/*******************************************:
 * コマンドの判別用関数
 * *****************************************/
char Wii_action::move_command(Wii_action::Prs prs)
{

    if (sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) < 0.40)
    {
        return STOP_COMMAND;
    }

    else if (prs.bal_y - prs.bal_x >= 0 && prs.bal_y + prs.bal_x >= 0 && sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) >= 0.40)
    {
        if (prs.bal_y >= 0.70)
        {
            return FORWARD_UP_COMMAND;
        }
        return FORWARD_COMMAND;
    }

    else if (prs.bal_y - prs.bal_x <= 0 && prs.bal_y + prs.bal_x <= 0 && sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) >= 0.40)
    {
        /**後側**/
        if (prs.bal_y <= -0.70)
        {
            return BACK_UP_COMMAND;
        }
        return BACK_COMMAND;
    }

    else if (prs.bal_y - prs.bal_x < 0 && prs.bal_y + prs.bal_x > 0 && sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) >= 0.40)
    {
        /**右側**/
        if (prs.bal_x >= 0.7)
        {
            return HIGH_RIGHT_TURN;
        }
        return RIGHT_TURN;
    }

    else if (prs.bal_y - prs.bal_x > 0 && prs.bal_y + prs.bal_x < 0 && sqrt(pow(prs.bal_x, 2) + pow(prs.bal_y, 2)) >= 0.40)
    {
        /**左側**/
        if (prs.bal_x <= -0.70)
        {
            return HIGH_LEFT_TURN;
        }
        return LEFT_TURN;
    }

    else
    {
        printf("NO COMMAND! Take stop.");
        return STOP_COMMAND;
    }
}
