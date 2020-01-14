#include "./WiiFit_common.h"
#include <SDL2/SDL.h>

struct cwiid_state Wii_action::state;
struct balance_cal Wii_action::balance_cal;

Wii_action::Wii_action()
{
}

void Wii_action::centroid(cwiid_wiimote_t *wiimote, Prs *prs)
{
    cwiid_get_state(wiimote, &state);             //状態の取得
    cwiid_get_balance_cal(wiimote, &balance_cal); //重さの調整

    prs->wlt = weight(state.ext.balance.left_top, balance_cal.left_top);
    prs->wrt = weight(state.ext.balance.right_top, balance_cal.right_top);
    prs->wlb = weight(state.ext.balance.left_bottom, balance_cal.left_bottom);
    prs->wrb = weight(state.ext.balance.right_bottom, balance_cal.right_bottom);
    prs->weight = prs->wlt + prs->wrt + prs->wlb + prs->wrb; //体重

    prs->bal_x = (prs->wrt + prs->wrb) / (prs->wlt + prs->wlb);
    if (prs->bal_x > 1)
        prs->bal_x = ((prs->wlt + prs->wlb) / (prs->wrt + prs->wrb) * (-1.0)) + 1.0;
    else
        prs->bal_x -= 1;

    prs->bal_y = (prs->wlt + prs->wrt) / (prs->wlb + prs->wrb);
    if (prs->bal_y > 1)
        prs->bal_y = ((prs->wlb + prs->wrb) / (prs->wlt + prs->wrt) * (-1.0)) + 1.0;
    else
        prs->bal_y -= 1;

    Client_command::Player_weight[Game::clientID] = prs->wlt + prs->wrt + prs->wlb + prs->wrb;
    SDL_Delay(50);
}

int Wii_action::move_command(Prs *prs)
{

    return 1;
}

double Wii_action::weight(uint16_t reading, uint16_t cal[3])
{
    /*	if (reading < cal[0])
		return 0.0;
*/ if (reading < cal[1])
        return ((double)reading - cal[0]) / (cal[1] - cal[0]) * 17.0;
    else
        return (((double)reading - cal[1]) / (cal[2] - cal[1]) * 17.0) + 17.0;
}
