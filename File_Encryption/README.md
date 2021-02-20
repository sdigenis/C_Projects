# File encryption

This project uses three different programs to list the files inside a 
directory (dirlist.c) to archive them (archive.c) and finally 
encrypt them (encrypt.c).  <br />

It also can reverse the procedure, by decrypting them (p2crypt.c) and
unachive them (unarchive). <br />

The main process open unamed pipes and starts the other programs.
It communicates with them via the pipes passing forward the information
from each program to the next one. <br />

The dirlist.c process takes as arguments the name of a directory and 
if there is such a directory it prints the files it contains
(not any subdirectories). <br />

The p2archive.c process reads from its input an array of file names and 
writes in the output for every file its name length, its name, a 
timestamp of when it was last accessed and modified, its file type and 
the file's permissions.  <br />
The p2unarchive process does the reverse thing writing out the files given
a data from the archived process. Though it also gets a parameter that will 
be a diretory's name where the files will be stored.  <br />

Finally the p2crypt.c process takes as argument a name of the encryption 
key and reads from its input the data it's going to encrypt. Finally it 
writes the encrypted data in its output. <br />
The encryption is a XOR method, so it can be used for decrypting the data
as well. <br />

In order to run the program you need to compile using the Makefile and then
you can run each process differently or combined by running the main process 
or by pipes through the terminal. <br />

A sample run would be: <br />
'''bash
./main -E dir password filename
'''
The first parameter can be either -E for archiving and encrypting and -D for 
decrypting and unarchiving. <br />

There is a problem with unarchive process as it won't work for all files. <br />

This project uses lots of C system calls for executing new processes 
(fork), redirecting inputs and outputs and connects them through 
unamed pipes.