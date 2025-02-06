cmake_minimum_required(VERSION 3.16)
project(hello VERSION 0.1.0 LANGUAGES C CXX)

# Set the compiler and linker sysroot so that headers and libraries are from chroot
# The -H flag is optional, it only shows where headers are found
# Note that core headers and and libs are still from x-tools because their
# versions are tightly bound to correct compiler operation
# --> every major rpi os upgrade requires new toolchain
set(RPI_SYSROOT /var/lib/schroot/chroots/rpi3-bookworm-armhf)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${RPI_SYSROOT} -H" CACHE INTERNAL "" FORCE)
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --sysroot=${RPI_SYSROOT}" CACHE INTERNAL "" FORCE)

set(SRC_FILES  
        /home/student/Desktop/embedded-linux-labs-przemyslaw/lab3/src/libgpiod-input.c         
        /home/student/Desktop/embedded-linux-labs-przemyslaw/lab3/src/libgpiod-led.c
) 

add_executable(lab3 ${SRC_FILES}) 


include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/local/include/) 
include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/include/) 
include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/include/arm-linux-gnueabihf/) 
target_link_libraries(lab3 /var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/lib/arm-linux-gnueabihf/libgpiod.so) 
