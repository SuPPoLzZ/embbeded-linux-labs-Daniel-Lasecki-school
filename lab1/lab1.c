#include <stdio.h>
#include <time.h>

int main(){
    FILE *output = fopen("output.txt", "a");
    time_t t = time(NULL);

    struct tm tm_info = *localtime(&t);

    fprintf(output, "Hello %04d-%02d-%02d \n", tm_info.tm_year + 1900, tm_info.tm_mon + 1, tm_info.tm_mday);
    
    fclose(output);
    return 0;

}