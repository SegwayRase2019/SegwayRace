/*
 * Copyright (C) 2011 by Daniel Friesel <derf@finalrewind.org>
 * License: WTFPL <http://sam.zoy.org/wtfpl>
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 */

/* Wii_main.c */

#include"Wii_common.h"
#include <unistd.h>
#include <time.h>
#include<bluetooth/bluetooth.h>

cwiid_mesg_callback_t cwiid_callback;


int main()
{
	cwiid_wiimote_t *wiimote = NULL;
	Prs prs;
	char command;

	if ((wiimote = cwiid_open(BDADDR_ANY, 0)) == NULL) {
		fputs("Unable to connect\n", stderr);
		return EXIT_FAILURE;
	}
	fputs("connected\n", stdout);

	sleep(1);

	if (cwiid_set_led(wiimote, 1))
		fputs("Unable to set LED state\n", stderr);

	if (cwiid_get_balance_cal(wiimote, &balance_cal))
		fputs("unable to retrieve balance calibration\n", stderr);

	if (cwiid_set_mesg_callback(wiimote, cwiid_callback))
		fputs("cannot set callback. buttons won't work.\n", stderr);
	
	if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC))
		fputs("cannot enable callback. buttons won't work.\n", stderr);

	if (cwiid_set_rpt_mode(wiimote,
			CWIID_RPT_ACC | CWIID_RPT_STATUS | CWIID_RPT_EXT))
		fputs("cannot set report mode. buttons won't work.\n", stderr);

	while (1) {
	prs = centroid(wiimote);
	command = move_command(prs);
	printf("command is %c\n",command);
	}

	return EXIT_SUCCESS;
}

void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
	union cwiid_mesg mesg[], struct timespec *ts)
{
	for (int i = 0; i < mesg_count; i++) {
		if (mesg[i].type == CWIID_MESG_BALANCE) {
		}
    }
}