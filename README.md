     ██████╗███████╗██╗  ██╗    ██╗  
    ██╔════╝██╔════╝██║  ██║    ╚██╗  
    ██║     ███████╗███████║     ╚██╗  
    ██║     ╚════██║██╔══██║     ██╔╝  
    ╚██████╗███████║██║  ██║    ██╔╝  
     ╚═════╝╚══════╝╚═╝  ╚═╝    ╚═╝  

ABOUT: A simple shell written in C. Project for OS Course at Florida Polytechnic University  
AUTHOR: Luis Moraguez

## Compiling
From within the project directory, do the folowing in the terminal:
```shell
mkdir ./bin
gcc -o ./bin/csh main.c csh.c -l readline
```
## Running
To run the project after compiling, do one of the following:
```
Interactive Mode:
./bin/csh

Batch Mode:
./bin/csh {batch_file}
```

## Built-ins
 - exit
 - cd
 - loop
 - path
 - export
 - help
 - about
 - history

 ### exit
   - Signal handling for `Ctrl-Z` and `Ctrl-C` are ignored for the shell itself, but automatically re-enabled for any child processes spawned from csh.
   - `exit` command without any arguments will close csh
   - `Ctrl-D` at prompt will also close csh

 ### cd
   - `cd {directory}` will change current working directory if destination exist
   - 0 arguments is invalid (requirements of project)
   - 1 argument with is correct
   - 2+ arguments is invalid

 ### loop
   - `loop {n} {command to execute} {optional: $LOOP}` will loop a given command n times specified.
   - Examples:
 ```
 Input:
 loop 3 echo hello world

 Output:
 hello world
 hello world
 hello world
 ------------------------------------------------

 Input:
 loop 3 echo hello world $LOOP

 Output:
 hello world 1
 hello world 2
 hello world 3
 ------------------------------------------------

 Input:
 loop 3 loop 2 echo hello world $LOOP

 Output:
 hello world 1
 hello world 2
 hello world 1
 hello world 2
 hello world 1
 hello world 2
 ```

 ### path
   - `path {directory_1} {directory_n}` Will set the $PATH environment variable using space-delimited directories

 ### export
   - `export {VARNAME} {VALUE}` Will set the named variable into the environment from the VALUE string provided

 ### help
   - `help` Lists the shell built-ins

 ### about
   - `about` Displays info about the shell

 ### history
   - `history` Will print list of command history
   - Pressing the 'up' or 'down' arrow keys during prompt will move user through history list one entry at a time

## Non-builtins
 - csh will let you execute non-builtin commands with ease because of its use of the 'execvp' syscall.

 ### Simple Commands
   - Simple non-builtin commands, such as `echo`, can be parsed an executed currently
 ### Looped Commands
   - Simple non-builtin commands can also be parsed and executed currently
 ### Redirection
   - Output redirection to a file currently works for both write and append
   - Input redirection (WIP)

## Action Items
 - [x] Add output redirection (write and append)
 - [ ] Add input redirection
 - [x] Add ability to set PATH and other environment variable using command similar to export (furture built-in)
 - [x] Ensure that complex command combinations can be parsed/executed, eg. `loop 5 echo world $LOOP >> ./result.txt`
 - [ ] Add piping

## Contributions:
Contributions are welcome. Fork the repo, make your changes, create a diff file, and email the diff file and your GitHub username to luis@moraguez.com. If the changes are approved, you will be added as a contributor to the repo.

## Donations:
If this utility helped you with a project you're working on and you wish to make a donation, you can do so by clicking the donate button that follows. Thank you for your generosity and support!

<noscript><a href="https://liberapay.com/z3d6380/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a></noscript>
