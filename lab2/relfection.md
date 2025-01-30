pi@pi:~/Desktop $ ls
hello  log_file.txt
pi@pi:~/Desktop $ tail -f log_file.txt 
Hello, i hope this runs at first try on arm processor 2025-01-30 08:20:47
pwd
^C
pi@pi:~/Desktop $ pwd
/home/pi/Desktop
pi@pi:~/Desktop $ file 
hello         log_file.txt  
pi@pi:~/Desktop $ file hello 
hello: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 5.15.138, with debug_info, not stripped


If i try to run the same file which is compiled for arm processor it will output an error for missing library

student@student-VirtualBox:~/Desktop/embedded-linux-labs-przemyslaw/lab2$ ./hello 
/lib/ld-linux-armhf.so.3: No such file or directory
student@student-VirtualBox:~/Desktop/embedded-linux-labs-przemyslaw/lab2$ file hello
hello: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 5.15.138, with debug_info, not stripped
student@student-VirtualBox:~/Desktop/embedded-linux-labs-przemyslaw/lab2$ 


