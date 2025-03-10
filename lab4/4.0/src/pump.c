#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <time.h>

#define CONSUMER "LED_ON"
#define LOG_FILE "water_log.txt"



void log_message(const char *message){
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file != NULL){
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        fprintf(log_file, "[%s] %s\n", asctime(tm_info), message);  // Write timestamp and message
        fclose(log_file);  
    } else {
        perror("Failed to open log file");
    }

}


int main(int argc, char **argv)
{
	char *chipname = "gpiochip0";
	unsigned int line_num = 23;	// GPIO Pin #23
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret, val;
    time_t t;
    struct tm *tm_info;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}

	// CHECK GPIO IF ITS HIGH OR LOW
    val = gpiod_line_get_value(line);
	gpiod_line_release(line);
    if (val == 1){
		log_message("WATER LEVEL HIGH PUMP ON");
		printf("WATER LEVEL HIGH PUMP ON");
	}else if(val == 0)
	{
		log_message("WATER LEVEL LOW PUMP OFF");
		printf("WATER LEVEL LOW PUMP OFF");
	}else{
		log_message("ERROR");
	}
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}