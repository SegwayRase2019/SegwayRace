#pragma once
extern "C" {
#include <cwiid.h>
}
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../common/Common.h"
#include "../main/Game.h"
#include "../component/InputComponent.h"
#include "../main/Client_func.h"

class Wii_action
{
public:
  Wii_action();

  static void centroid(cwiid_wiimote_t *wiimote, Prs *prs);
  static int move_command(Prs *prs);

private:
  static struct cwiid_state state;
  static struct balance_cal balance_cal;
  static double weight(uint16_t reading, uint16_t cal[3]);
};