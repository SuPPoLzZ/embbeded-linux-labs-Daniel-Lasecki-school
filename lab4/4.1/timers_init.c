#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <gpiod.h>
#include <string.h> //needed by memset
#include <unistd.h> // For sleep

static int gpio_initialized = 0; 

/*global timer objects */
timer_t firstTimerID;
timer_t pulseTimerID;

/* Forward declarations */
static void timerHandler(int sig, siginfo_t *si, void *uc);
static void pulseStopHandler(int sig, siginfo_t *si, void *uc);

static struct gpiod_line *line = NULL; 
static struct gpiod_chip *chip = NULL; 

/**@brief Function for creating a timer
 *
 * @details
 */
static int makeTimer(timer_t *timerID, int expire_usec, int interval_usec)
{
    struct sigevent te;
    struct itimerspec its;
    struct sigaction sa;
    int sigNo = SIGRTMIN;

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = (interval_usec == 0) ? pulseStopHandler : timerHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sigNo, &sa, NULL) == -1) {
        printf("Failed to setup signal handling for timer\n");
        return (-1);
    }

    /* Set and enable alarm */
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    if (timer_create(CLOCK_REALTIME, &te, timerID) == -1) {
        printf("Failed to create timer\n");
        return (-1);
    }

    its.it_value.tv_sec = expire_usec / 1000000;  // microseconds to seconds
    its.it_value.tv_nsec = (expire_usec % 1000000) * 1000; // remaining microseconds to nanoseconds
    its.it_interval.tv_sec = interval_usec / 1000000;  // microseconds to seconds
    its.it_interval.tv_nsec = (interval_usec % 1000000) * 1000; // remaining microseconds to nanoseconds

    if (timer_settime(*timerID, 0, &its, NULL) == -1) {
        printf("Failed to set timer\n");
        return (-1);
    }

    return(0);
}

/**@brief Common handler for all the timers
 *
 * @details
 */
static void timerHandler(int sig, siginfo_t *si, void *uc)
{
    timer_t *tidp;
    tidp = si->si_value.sival_ptr;

    // Open GPIO chip only once, and store the reference
    if (!gpio_initialized) {
        chip = gpiod_chip_open("/dev/gpiochip0"); 
        if (!chip) {
            printf("Failed to open GPIO chip\n");
            return;
        }
        line = gpiod_chip_get_line(chip, 23);  
        if (!line) {
            printf("Failed to get GPIO line\n");
            return;
        }
        if (gpiod_line_request_output(line, "PWM_Pulse", 0) == -1) {
            printf("Failed to request GPIO line as output\n");
            return;
        }
        gpio_initialized = 1; // Set the flag to indicate GPIO is initialized
        printf("GPIO line requested as output\n");
    }

    if (*tidp == firstTimerID) {
        printf("Setting GPIO line high\n");
        if (line) {
            gpiod_line_set_value(line, 1);
        } else {
            printf("Error: GPIO line is NULL\n");
        }
        makeTimer(&pulseTimerID, 1500, 0); // Create a one-shot timer for 1500 microseconds
    }
}

/**@brief Handler for the pulse stop timer
 *
 * @details
 */
static void pulseStopHandler(int sig, siginfo_t *si, void *uc)
{
    timer_t *tidp;
    tidp = si->si_value.sival_ptr;

    if (!gpio_initialized) {
        chip = gpiod_chip_open("/dev/gpiochip0"); 
        if (!chip) {
            printf("Failed to open GPIO chip\n");
            return;
        }
        line = gpiod_chip_get_line(chip, 23);  
        if (!line) {
            printf("Failed to get GPIO line\n");
            return;
        }
        if (gpiod_line_request_output(line, "PWM_Pulse", 0) == -1) {
            printf("Failed to request GPIO line as output\n");
            return;
        }
        gpio_initialized = 1; // Set the flag to indicate GPIO is initialized
        printf("GPIO line requested as output\n");
    }

    
    if (line) {
        gpiod_line_set_value(line, 0);
    } 
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. Also creates application timers.
 * @todo 	Revise error handling
 */
int timers_init(void)
{
    int err_code;

    // Create timers
    err_code = makeTimer(&firstTimerID, 2, 2); // 20ms interval timer
    if (err_code != 0) {
        return err_code;
    }

    return 0;
}