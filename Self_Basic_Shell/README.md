# Self Basic Shell

This project is an implementation of a basic shell, where 
processes are running, in a round-robin algorithm, with 
them switching either by running enough time (20s) or by 
handling a signal. <br />

The project can be compiled through the Makefile. <br />

In order to run it you just need to run the shell binary 
file.   <br /> 
```bash
./shell 
```
From now on every 20s there is going to be a BEEP message 
indecating the alarm signaling the switch of processes.
At the beggining there are no processes running. <br />
While the process is running you can execute some commands 
there.  <br />
Specifically you can run commands to execute a program and 
terminate or signal a specific process that runs, as well as
listing all the processes that are currently running. <br />
 <br />
Executing a program command: <br />
&emsp;   exec <program name> <args>  <br />
Terminating a process command:  <br />
&emsp;   term <process id>  <br />
Signal a process: <br />
&emsp;   sig <process id> <br />
Listing all the ids of the processes running: <br />
&emsp;   list <br />
Quiting the shell: <br />
&emsp;   quit <br />

In order to test the function of the project there is the 
test.c file. Its function is to raise a counter every 5 
seconds and printing its pid, the counter that indicates how 
many times it has runned and a number given in the arguments 
that is the maximum times it has to run.  <br />
This program is compiled as well in the makefile. <br />

In order to run it you have to give these arguments: <br />
```bash
    ./test -m #maximum number of iterations -b 0 
```
the final argument can be either 0 or 1 indicating if it
should get blocked by SIGUSR1 or not. If 1 then it gets blocked
for the its first half iterations and then it gets unblocked 
for the remaining time. <br />

This project was made to fully understand and get some experience 
on signals, signal handling, how a shell works and how a scheduler 
function with the processes running. <br />