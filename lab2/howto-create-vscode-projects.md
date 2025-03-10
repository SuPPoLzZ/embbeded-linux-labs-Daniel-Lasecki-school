# Native and cross build projects in vscode

C source code is portable in the sense that it does not dictate what platform or architecture it is compiled to. Native build results in executable code in same architecture as the build architecture (in our x86 VM we build native x86 executables). Cross-build refers to code built for another architecture (in our x86 VM we build executables for arm architecture).

Vscode can be configured to both targets. Native build setup was covered in lab1.

## Cross-build C projects

There are no prebuilt templates for specific cross-development setups like our arm-raspi lab setup. The easiest way to proceed is to copy setup files from existing project (lab2 for example)
- .vscode/* 
- cmake/*
- CMakeLists.txt  
Then
- edit CMakeLists.txt so that source files and library dependencies match your new project 
- Shift-Ctrl-P: "CMake: Delete Cache and Reconfigure" to rebuild CMake cache. Choose kit (toolchain) "GCC 13.2.0 armv8-rpi3-linux-gnueabihf"  
You should now be able to do cross-builds and remote debugging with the new project.  
Note that key shortcut are different for CMake projects:  
`F7` to build (especially note that Shift-Ctrl-B builds with wrong parameters... annoying)  
`F5` to debug
> For these to work, vscode needs to opened in folder ~/embedded-linux-labs/lab2 or whatever is the place for your current project. This you can do either  
> a) by starting vscode from command line, like in `student@student-VirtualBox:~/embedded-linux-labs/lab2$ code .` or  
> b) using vscode menu File -- Open Folder to choose current project  
> Additionally, you should enable git setting "Open Repository in Parent Folders" to enable fluent git usage from current project folder

