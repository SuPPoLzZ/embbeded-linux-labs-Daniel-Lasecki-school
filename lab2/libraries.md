Open another raspi terminal to tail -f your code output:

pi@pi:~/Desktop $ tail lab2.log 
I'm logging 0 ...
I'm logging 1 ...
I'm logging 2 ...
I'm logging 3 ...
---------------
file path pi:

pi@pi:~ $ find ~ -name "hello"
/home/pi/Desktop/hello

file path ubuntu:
student@student-VirtualBox:~/Desktop/embedded-linux-labs/lab2$ find ~ -name "hello"
/home/student/.local/share/Trash/files/hello
/home/student/Desktop/embedded-linux-labs/lab2/hello


----------
use ldd command to find out what shared runtime libraries your executable requires (you need to run this in environment that supports the architecture of previous step!):
raspberry:

pi@pi:~/Desktop $ ldd hello
/usr/lib/arm-linux-gnueabihf/libarmmem-${PLATFORM}.so => /usr/lib/arm-linux-gnueabihf/libarmmem-v8l.so (0xf7eb0000)
libc.so.6 => /lib/arm-linux-gnueabihf/libc.so.6 (0xf7d25000)
/lib/ld-linux-armhf.so.3 (0xf7ece000)


----------------------

library architecture (use file as in cli commands above; you may need to follow symlinks):

pi@pi:~/Desktop $ file hello
hello: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3, for GNU/Linux 5.15.138, with debug_info, not stripped


-------------
Find all files named libc.so.6 in both systems VM and raspi (use find command; use sudo to extend the search to all directories, do net search to get command parameters right)

ubuntu:

student@student-VirtualBox:~/Desktop/embedded-linux-labs/lab2$ sudo find / -name "libc.so.6" 2>/dev/null
/usr/lib/x86_64-linux-gnu/libc.so.6
/usr/libx32/libc.so.6
/usr/arm-linux-gnueabi/libhf/libc.so.6
/usr/arm-linux-gnueabihf/lib/libc.so.6
/usr/lib32/libc.so.6
/usr/aarch64-linux-gnu/lib/libc.so.6
/var/lib/schroot/chroots/rpizero-bullseye-armhf/usr/lib/arm-linux-gnueabihf/libc.so.6
/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/lib/arm-linux-gnueabihf/libc.so.6
/var/lib/schroot/chroots/rpizero-buster-armhf/usr/lib/arm-linux-gnueabihf/libc.so.6
/home/student/opt/x-tools/aarch64-rpi3-linux-gnu/aarch64-rpi3-linux-gnu/sysroot/lib/libc.so.6
/home/student/opt/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot/lib/libc.so.6
/home/student/opt/x-tools/armv8-rpi3-linux-gnueabihf/armv8-rpi3-linux-gnueabihf/sysroot/lib/libc.so.6
/snap/snapd/23545/usr/lib/x86_64-linux-gnu/libc.so.6
/snap/core20/2434/usr/lib/i386-linux-gnu/libc.so.6
/snap/core20/2434/usr/lib/x86_64-linux-gnu/libc.so.6
/snap/core22/1722/usr/lib/i386-linux-gnu/libc.so.6
/snap/core22/1722/usr/lib/x86_64-linux-gnu/libc.so.6

raspberry:
pi@pi:~/Desktop $ sudo find / -name "libc.so.6" 2>/dev/null
/usr/lib/arm-linux-gnueabihf/libc.so.6
pi@pi:~/Desktop $ 

---------------

Find all files named libc.so.6 in both systems VM and raspi (use find command; use sudo to extend the search to all directories, do net search to get command parameters right):


student@student-VirtualBox:~/Desktop/embedded-linux-labs/lab2$ ./glibc_version 
Compile-time glibc version: 2.31
Runtime glibc version: 2.31

pi@pi:~ $ ./glibc_version 
Compile-time glibc version: 2.36
Runtime glibc version: 2.36




Reflection: What are the key benefits of cross-development setup?

Key benefit is that you can use ide (vscode) and compile it on much powerful computer and just copy it into embbeded system which often dont have any peripherals or enough computing power.






