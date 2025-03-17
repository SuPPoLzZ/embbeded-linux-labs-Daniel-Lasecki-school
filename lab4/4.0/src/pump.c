#include <stdio.h>
#include <gpiod.h>

int main() {
    char *chipname = "gpiochip0";  
    unsigned int line_num = 17;   // GPIO 17 is the input for detecting the LED state
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret, val;

    // Open the GPIO chip
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Open chip failed");
        return 1;
    }

	// Request GPIO 17 as input
	line = gpiod_chip_get_line(chip, 17);
	ret = gpiod_line_request_input(line, "LED Detection");
	if (ret < 0) {
		perror("Failed to request GPIO 17 as input");
		gpiod_chip_close(chip);
		return 1;
	}

	// Read the value of GPIO 17
	val = gpiod_line_get_value(line);
	if (val == -1) {
		perror("Failed to read GPIO 17");
	} else {
		if (val == 1) {
			printf("WATER PUMP ON\n");
		} else {
			printf("WATER PUMP OFF\n");
		}
	}

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return 0;
}
