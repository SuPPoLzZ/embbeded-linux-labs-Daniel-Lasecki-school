cmake_minimum_required(VERSION 3.16)
project(hello VERSION 0.1.0 LANGUAGES C CXX)

set(RPI_SYSROOT /var/lib/schroot/chroots/rpi3-bookworm-armhf)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${RPI_SYSROOT} -H" CACHE INTERNAL "" FORCE)
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --sysroot=${RPI_SYSROOT}" CACHE INTERNAL "" FORCE)

include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/local/include/) 
include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/include/) 
include_directories(/var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/include/arm-linux-gnueabihf/)

# Define the executable first
add_executable(lab4.0_LED_ON src/led_on.c)
add_executable(lab4.0_LED_OFF src/led_off.c)
add_executable(lab4.0_WATER src/pump.c)


# Then link the libraries
#target_link_libraries(lab4.1_LED_ON /var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/lib/arm-linux-gnueabihf/libgpiod.so) 
#target_link_libraries(lab4.1_LED_OFF /var/lib/schroot/chroots/rpi3-bookworm-armhf/usr/lib/arm-linux-gnueabihf/libgpiod.so) 
target_link_libraries(lab4.0_LED_ON gpiod)
target_link_libraries(lab4.0_LED_OFF gpiod)
target_link_libraries(lab4.0_WATER gpiod)