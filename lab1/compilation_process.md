### Step 5: Compilation and execution process

The general process from C source to a running program has four preparation steps:
1. <b>Preprocessor</b> processes one C source file at a time: for each it expands macros (#define's), inserts contents of included files (#include's) and produces intermediate file also known as <b>translation unit</b>. Each translation unit is compiled separately.
2. <b>Compiler</b> translates C source from each translation unit into target machine code, resulting in <b>object file</b>. Object files contain machine instructions and debug symbols, but they have no set links to any libraries or other translation units. You can not run object files.
3. <b>Linker</b> merges multiple object files adding address bindings between them, and also to <b>static libraries</b> which are permanently added to same output file. This output file is <b>executable binary</b> or <b>executable</b>, and it typically contains debug symbols in ELF format.
4. <b>Loader</b> loads the executable from mass memory (HDD or SSD) into main memory (DRAM), and links the code to <b>dynamic libraries</b> (Windows DLL) / <b>shared libraries</b> (Linux .so) in the system. Dynamic libraries aim to minimise the main memory usage for the complete system: If all running programs use printf for output, it is not necessary to load a separate library instance for every application. The applications can share one dynamic library. Loader makes sure all required shared libraries are loaded before the executable is started. Note that in <b>bare-metal systems</b> there is no OS and no loader, so linker must prepare complete system memory image.

The common tool for C compilation in Linux is `gcc`. It is user-friendly by default, and runs steps 1-3 automatically if possible. It is still necessary to understand the process, because the understanding the process makes compilation error messages more understandable.

### Lab assignment: native build on command line:

"Native" refers to setup where you build on intel processor and produce intel code, and you can run that code in the VM.
In VM in your local repository folder (embedded-linux-labs/lab1),
1. open terminal in subfolder lab1 and do following work in it:
2. write tiny C application named lab1.c that prints timestamped hello message to an output file (use append). Can use nano editor, try "nano lab1.c"
3. compile the code using VM native gcc (producing x86 architecture executable) `gcc lab1.c -o lab1`
4. use command `file lab1` to check that the executable is built for x86(-64) architecture
5. open two terminals, one with `tail -f your_output_filename` and in other you run your application.
6. check that your results fullfill the requirements above (2.)
7. check git status
8. add source file to git (only sources go to version control, so create .gitignore file and add executable `lab1` there), commit
9. push local repository changes to your remote repository in TUAS gitlab 

Reflection: In embedded application, why would you write application output to a file instead of just printf it to terminal? Write answer to markdown file reflection.md; commit and push.

## Native C projects in vscode

Next you will want to build and run the code from vscode.

### Project setup (C project with one source file)

A minimal setup consists of setting up two files in the project:
- ```.vscode/tasks.json``` to define build task (build task is bound to hotkey Shift-Ctrl-B) so that gcc is invoked on currently active code tab
- ```.vscode/launch.json``` to define debug task (F5 key) to launch gdb

The base version for tasks.json
```
//tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "gcc",
            "args": [
                "-g",
                "${file}",
                "-o",
                "${fileBasenameNoExtension}"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```
If you need to add external libraries or paths, add those to argument list (remember to append a comma to previous item to keep list format valid)
```
                "-l", "paho-mqtt3c"
```

The base version for launch
```
//launch.json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "gcc - Build and debug active file",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build",
            "miDebuggerPath": "/usr/bin/gdb"
        }
    ]
}
```

In nutshell:
- Open an empty folder for the new project (File > Open Folder...)
- add New File (hello.c or whatever)
- Shift-ctrl-P: C/C++: Edit Configurations (JSON)   --> creates file .vscode/c_cpp_properties.json
- select your source file tab again
- to build: Shift-Ctrl-B: choose "gcc build active file"
- to debug F5: choose "C++ (GDB/LLDB)", choose "gcc build and debug active file"
The tool selections are stored in .vscode/tasks.json
You can now set breakpoints into source file and start debugging the code (F5).

### Project setup (multiple source files)

You would need to set up make or CMake project for this. Easiest way to go is to copy .vscode/tasks.json configuration (and possibly other files) from an other existing project.

### Lab exercise

Set up tasks.json and launch.json for lab1 and check that you can build the code with ctrl-shift-B and debug it using F5 (you need to set a breakpoint in the code before debugging, otherwise it just runs through)