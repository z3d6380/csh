     ██████╗███████╗██╗  ██╗    ██╗  
    ██╔════╝██╔════╝██║  ██║    ╚██╗  
    ██║     ███████╗███████║     ╚██╗  
    ██║     ╚════██║██╔══██║     ██╔╝  
    ╚██████╗███████║██║  ██║    ██╔╝  
     ╚═════╝╚══════╝╚═╝  ╚═╝    ╚═╝  

ABOUT: A simple shell written in C. Project for OS Course at Florida Polytechnic University  
AUTHOR: Luis Moraguez

## Built-ins
 - exit
 - cd
 - loop
 - help
 - about
 - history

 ### exit
 - - Signal handling for `Ctrl-Z` and `Ctrl-C` are ignored for the shell itself, but automatically re-enabled for any child processes spawned from csh.
 - - `exit` command without any arguments will close csh
 - - `Ctrl-D` at prompt will also close csh

 ### cd
 - - `cd {directory}` will change current working directory if destination exist
 - - 0 arguments is invalid (requirements of project)
 - - 1 argument with is correct
 - - 2+ arguments is invalid

 ### loop
 - - `loop {n} {command to execute} {optional: $LOOP}` will loop a given command n times specified.
 - - Examples:
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
 ### help
 - - `help` Lists the shell built-ins

 ### about
 - - `about` Displays info about the shell

 ### history
 - - `history` Will print list of command history
 - - Pressing the 'up' or 'down' arrow keys during prompt will move user through history list one entry at a time

## Non-builtins
 - csh will let you execute non-builtin commands with ease because of its use of the 'execvp' syscall.

 ### Simple Commands
 - - Simple non-builtin commands, such as `echo`, can be parsed an executed currently
 ### Looped Commands
 - - Simple non-builtin commands can also be parsed and executed currently
 ### Redirection (WIP)
 - - Output redirection to a file is currently not implemented
 ### Piped Commands (WIP)
 - - Piping commands is currently not implemented

## Action Items
 - [ ] Add output redirection (write and append)
 - [ ] Add command piping
 - [ ] Add ability to set PATH and other environment variable using command similar to export (furture built-in)
 - [ ] Ensure that complex command combinations can be parsed/executed, eg. `loop 5 echo hello world $LOOP | grep world >> ./result.txt`