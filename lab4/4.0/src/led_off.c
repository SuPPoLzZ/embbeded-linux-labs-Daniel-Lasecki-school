#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>
#include <time.h>


#define CONSUMER "LED_OFF"
#define LOG_FILE "led_log.txt"



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
	int i, ret;
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

	ret = gpiod_line_request_output(line, CONSUMER, 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		goto release_line;
	}
	
    gpiod_line_set_value(line, 1);
    printf("LED OFF" );
    log_message("LED OFF ");
    sleep(10);

	

release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}