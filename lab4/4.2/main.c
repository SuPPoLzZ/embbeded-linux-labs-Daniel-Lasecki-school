#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define PWM_PERIOD_PATH "/sys/class/pwm/pwmchip0/pwm0/period"
#define PWM_DUTY_PATH "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define PWM_ENABLE_PATH "/sys/class/pwm/pwmchip0/pwm0/enable"

void pwm_handler(int sig);
void set_pwm_period();
void set_pwm_enable();
void set_pwm_duty(int duty);
void setup_timer();
void init_pwm();

int s_curve[101];
volatile sig_atomic_t stop_timer = 0;  // Flag to stop the timer after 100%

int main() {
    init_pwm();
    setup_timer();

    // Initialize the S-curve duty cycle values (0% = 1.2ms, 100% = 1.8ms)
    for (int i = 0; i <= 100; i++) {
        s_curve[i] = 1200000 + (i * (1800000 - 1200000)) / 100;
    }

    while (!stop_timer) {
        pause();  
    }

    printf("Duty cycle completed succesfully\n");
    return 0;
}

void init_pwm() {
    set_pwm_period();
    set_pwm_enable();
}

void set_pwm_period() {
    FILE *pwm_period = fopen(PWM_PERIOD_PATH, "w");
    if (!pwm_period) {
        perror("Failed to open PWM period file");
        exit(1);
    }
    fprintf(pwm_period, "20000000");
    fflush(pwm_period);
    fclose(pwm_period);
}

void set_pwm_enable() {
    FILE *pwm_enable = fopen(PWM_ENABLE_PATH, "w");
    if (!pwm_enable) {
        perror("Failed to open PWM enable file");
        exit(1);
    }
    fprintf(pwm_enable, "1");
    fflush(pwm_enable);
    fclose(pwm_enable);
}

void set_pwm_duty(int duty) {
    FILE *pwm_duty = fopen(PWM_DUTY_PATH, "w");
    if (!pwm_duty) {
        perror("Failed to open PWM duty cycle file");
        exit(1);
    }
    fprintf(pwm_duty, "%d", duty);
    fflush(pwm_duty);
    fclose(pwm_duty);
}

void pwm_handler(int sig) {
    static int idx = 0;

    int duty_cycle_ns = s_curve[idx];
    int percentage = (duty_cycle_ns - 1200000) * 100 / 600000;

    set_pwm_duty(duty_cycle_ns);

    printf("PWM Duty Cycle: %d%%\n", percentage);

    idx++;

    if (idx > 100) {
        stop_timer = 1;  // Set the flag to stop the timer
        printf("Duty cycle completed, stopping timer...\n");
    }
}

void setup_timer() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = pwm_handler;
    sigaction(SIGALRM, &sa, NULL);

    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100000;  // 100ms interval
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100000;  // 100ms repeating interval

    setitimer(ITIMER_REAL, &timer, NULL);
}
