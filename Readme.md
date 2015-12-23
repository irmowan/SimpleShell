# Simple Shell
This is a simple shell implemented by C language, as a course project of Operating System.

When you run the shell, you can see this prompt: 
```
Welcome to irmo's simple shell!
```
The shell provides these features as follows:

- The shell parses the input command by spaces. It can tolerate spaces at the beginning or ending of the command, and more than one spaces between two parameters in the command. But it may not work when spaces appear in the quotation marks.
- When you want to quit, you can use the command `exit`.
- The shell can send an `SIGINT` signal, so you can use `Ctrl+C` to exit the shell as well.
- As the shell uses the system call `execvp`, most commands that can run in a practical shell also work in this simple shell.
- If you want to run the command in the background, you can append an ampersand `&` as the last parameter. For example,  `cat SimpleShell.c &`. In this case, the shell will not wait this child process.
- The shell implements the command `history`, to show history commands. Moreover, you can use command `!!`, `!N`(here N is an integer from 1 to 10), the shell will search the given command in history, and then executes it.
- In addition, the shell also realizes the command `cd`, and supports some special types of calling like `cd ~`, `cd /`, `cd ~/Documents`, `cd ../..`.
- The shell allows commands with syntax error and do not crash. It will return you messages like `Command not found`,  `No such file or directory`,  `No such command in history` and some others when you input illegal commands like `lss`, `!abc`.

Here is a demo you can refer to:
```shell
Welcome to irmo's simple shell!
osh >   ls
CMakeLists.txt	Readme.md	SimpleShell	SimpleShell.c
osh > ls  -m
CMakeLists.txt, Readme.md, SimpleShell, SimpleShell.c
osh >    cd ~
osh > ls
Applications	Downloads	Music		Public
Desktop		Library		Pictures	PycharmProjects
Documents	Movies		Projects	Sites
osh > cd ~/Documents/OS/SimpleShell
osh >  history
6  history
5 cd ~/Documents/OS/SimpleShell
4 ls
3    cd ~
2 ls  -m
1   ls
osh >  !4
ls
CMakeLists.txt	Readme.md	SimpleShell	SimpleShell.c
osh > !0
No such command in history.
osh > ls &
osh > CMakeLists.txt	Readme.md	SimpleShell	SimpleShell.c
git branch
* master
osh > exit
```
Have fun with the shell!

---
Irmo   
2015.9
