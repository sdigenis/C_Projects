# Flight Booking Service

This project implements a flight tickets booking service locally 
in a computer. A server gets requests from agents for access to 
shared memory where it's stored the different flights information
and available tickets in each one. The server also gets tickets 
booking requests from agents.  <br />
Each agent, when given access to the shared memory, he can read 
the available flights or write in it in order to reserve a ticket.
There can be lots of agents at the same time in the shared memory and
they can read it, though writing is under mutual exclusion with the help 
of semaphores. <br />

In order to run the project you have to compile it using the makefile.
Then the server gets as arguments the maximum number of agents it can 
handle, a file that contains the flight information (flights directory)
and a key that will be neeeded to the agents to connect. <br />

An example would be: 
```bash
    ./server 4 flights/flightInfoShort.txt key
```
The agent process runs the agent process, which gets as arguments the 
key of the server's communication. <br />

An example would be:
```bash
    ./agent key
```
When the server starts running it waits an agent to connect. When a new 
agent connects it will give a message saying that a specific agent connected.
Also when a new reservation will be made there is a message to the server 
saying how many tickets the agent reserved. Finally when an agent process is 
stopped a new message will be prompted to the server saying which agent 
disconnected and how many tickets he has reserved. <br />

An agent when he gets the confirmation to enter the shared memory (if there 
are already enough agents connectes there will be a message that the system is
full and it will end the process) there will this menu on the screen  <br />
&emsp   FIND RESERVE EXIT
 <br />
there the agent can find a specific flight by giving the command <br />
&emsp   f SRC DEST #number of tickets  <br />
ex <br />
&emsp   f ATH FRA 3 <br />
and he will get a reply in which airlines have this flight and how available 
tickets there are in each airline. <br />
Then he can book the tickets by running the command <br />
&emsp   r SRC DEST AIRLINE #number of tickets <br />
ex. <br />
&emsp   r ATH FRA A3 3 <br />
and if there are enough tickets he will get a message that he indeed made the 
reservation. <br />
In order to exit the agent just needs to run the command  <br />
&emsp   e

After all the clients have exited the server, the server process ends as well 
printing how many tickets in total have been booked. <br />

There is a problem in reading big files, as when giving as an argument the file 
that contains lots of flights information there is a segmetation fault.  <br /> 

This assignment is a complete understanding of socket communication, shared memory,
semaphores and polling. <br />