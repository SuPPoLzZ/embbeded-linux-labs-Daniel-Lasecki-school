cmake_minimum_required(VERSION 3.16)
project(hello VERSION 0.1.0 LANGUAGES C CXX)

set(RPI_SYSROOT /var/lib/schroot/chroots/rpi3-bookworm-armhf)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${RPI_SYSROOT} -H" CACHE INTERNAL "" FORCE)
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --sysroot=${RPI_SYSROOT}" CACHE INTERNAL "" FORCE)

include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/local/include/) 
include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/include/) 
include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/include/arm-linux-gnueabihf/) 

set(SRC_FILES  
    /home/student/Desktop/embedded-linux-labs-przemyslaw/lab5/src/event_test_with_threads_debounce.c      
    /home/student/Desktop/embedded-linux-labs-przemyslaw/lab5/src/event_test_with_threads.c
    /home/student/Desktop/embedded-linux-labs-przemyslaw/lab5/src/main.c
    /home/student/Desktop/embedded-linux-labs-przemyslaw/lab5/src/event_test_without_threads.c
) 

# Define the executable first
add_executable(lab5 ${SRC_FILES}) 

# Then link the libraries
target_link_libraries(lab5 /var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/lib/arm-linux-gnueabihf/libgpiod.so) 
