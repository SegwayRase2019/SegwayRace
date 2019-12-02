/**common.h**/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<cwiid.h>

#define FORWARD_COMMAND    '1'
#define FORWARD_UP_COMMAND '2'
#define BACK_COMMAND       '3'
#define BACK_UP_COMMAND    '4'
#define RIGHT_TURN         '5'
#define HIGH_RIGHT_TURN    '6'
#define LEFT_TURN          '7'
#define HIGH_LEFT_TURN     '8'
#define STOP_COMMAND       '9'

typedef struct{
	double wlt;
	double wrt;
	double wlb;
	double wrb; 
	double bal_x;
	double bal_y;
}Prs;

struct balance_cal balance_cal;
struct cwiid_state state;

/* connect.c */
int main();
void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], struct timespec *ts);

/* calculate.c */
double weight(uint16_t reading, uint16_t cal[3]);
Prs centroid(cwiid_wiimote_t *wiimote);

/*command.c*/
char move_command(Prs prs);


