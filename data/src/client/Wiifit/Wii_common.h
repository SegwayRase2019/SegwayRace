/**common.h**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
extern "C"{
#include <cwiid.h>
}

#define FORWARD_COMMAND '1'
#define FORWARD_UP_COMMAND '2'
#define BACK_COMMAND '3'
#define BACK_UP_COMMAND '4'
#define RIGHT_TURN '5'
#define HIGH_RIGHT_TURN '6'
#define LEFT_TURN '7'
#define HIGH_LEFT_TURN '8'
#define STOP_COMMAND '9'

class Wii_action
{

  public:

	typedef struct
	{
		float wlt;
		float wrt;
		float wlb;
		float wrb;
		float bal_x;
		float bal_y;
	}Prs;

	struct balance_cal balance_cal;
	struct cwiid_state state;

	int Wii_main(char *argv[]);

	/* calculate.c */
	void centroid(cwiid_wiimote_t *wiimote,Wii_action::Prs *prs);

	/*command.c*/
	char move_command(Wii_action::Prs prs);

  private:

	/* calculate.c */
	double weight(uint16_t reading, uint16_t cal[3]);
	/* connect.c */
};
