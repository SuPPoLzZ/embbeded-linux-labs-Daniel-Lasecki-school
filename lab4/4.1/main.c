/*
 * main.c
 *
 *  Created on: 17 Sep 2019
 *      Author: Jarno Tuominen
 */


#include "timers_init.h"
#include <stdio.h> //Needed for fprintf
#include <unistd.h> //needed for sleep
#include <gpiod.h> // gpio library
#include <time.h>

int main(void)
{
    char *chipname = "gpiochip0";
    unsigned int line_num = 23; // GPIO Pin #23
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int i, ret;
    time_t t;
    struct tm *tm_info;
    int err_code;

   
    err_code = timers_init();
    if (err_code != 0) {
        gpiod_chip_close(chip);
        return err_code;
    }

    

    while(1) {
        sleep(1);
    }

    
    gpiod_chip_close(chip);
    return 0;

}
