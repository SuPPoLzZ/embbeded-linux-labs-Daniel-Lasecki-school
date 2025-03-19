/*
 * timers_init.c
 *
 *  Created on: 17 Sep 2019
 *      Author: Jarno Tuominen
 */


/* Timer initialization routines and handlers */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h> //needed by memset

/* Let's create the global timer objects */
//timer_t firstTimerID;
//timer_t secondTimerID;
//timer_t thirdTimerID;
timer_t fourthTimerID;

/**@brief Common handler for all the timers
 *
 * @details
 */
static void timerHandler( int sig, siginfo_t *si, void *uc )
{
    timer_t *tidp;
    FILE *fp;
    static struct gpiod_line *line; // Reference to GPIO line for toggling
    tidp = si->si_value.sival_ptr;

    // Open GPIO chip only once, and store the reference
    if (!line) {
        line = gpiod_chip_get_line(chip, line_num);  
        if (!line) {
            printf("Failed to get GPIO line\n");
            return;
        }
        gpiod_line_request_output(line, "PWM_Pulse");
    }

   
    gpiod_line_set_value(line, 1); 
    usleep(1500); 
    gpiod_line_set_value(line, 0);

    
    fp = fopen("/home/pi/timers.log", "a");
    if (*tidp == fourthTimerID) {
        fprintf(fp, "TIMER 4 SAYS HELLO WORLD\n");
    }
    fclose(fp);
}

/**@brief Function for creating a timer
 *
 * @details
 */
static int makeTimer(timer_t *timerID, int expire_msec, int interval_msec )
{
    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int sigNo = SIGRTMIN;

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timerHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sigNo, &sa, NULL) == -1) {
    	printf ("Failed to setup signal handling for timer\n");

        return(-1);
    }

    /* Set and enable alarm */
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);

    its.it_value.tv_sec = expire_msec / 1000000;  // microseconds to seconds
    its.it_value.tv_nsec = (expire_msec % 1000000); // remaining microseconds to nanoseconds
    its.it_interval.tv_sec = interval_msec / 1000000;  // microseconds to seconds
    its.it_interval.tv_nsec = (interval_msec % 1000000); // remaining microseconds to nanoseconds

    timer_settime(*timerID, 0, &its, NULL);
    return(0);
}



/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. Also creates application timers.
 * @todo 	Revise error handling
 */
int timers_init(void)
{
	int err_code;

	//Create some timers

	// err_code = makeTimer(&firstTimerID, 5e3, 5e3); //5s
	// if (err_code != 0) {
	// 	return err_code;
	// }
	// err_code = makeTimer(&secondTimerID, 3e3, 3e3); //3s
	// if (err_code != 0) {
	// 		return err_code;
	// }
	// err_code = makeTimer(&thirdTimerID, 15e3, 15e3); //15s
	// if (err_code != 0) {
	// 		return err_code;
	// }

    err_code = makeTimer(&fourthTimerID, 1500, 20000);
    if (err_code != 0) {
        return err_code;
    }

	return 0;

}
